"""TCP server module for CSCI 460 Assignment 1

Copyright (C) 2013  Tristan W. Bonsor

Multithreaded server module. Extends base classes provided by SocketServer
module. Incoming client connections are maintained until quit request is sent by
the client, or shutdown is initiated by the server.

"""

import re
import socket
import SocketServer
import sys
import threading

import define

class RequestHandler(SocketServer.BaseRequestHandler):

    """Handler class for incoming client connections to server."""

    def get_data(self, nbytes):

        """Recieve data from request source (blocking)."""

        self.data = self.request.recv(nbytes)

    def send_data(self, data):

        """Send data to request source."""

        self.request.sendall(data)

    def send_clients(self, original):

        """Send client list to request source."""

        self.send_data(self.server.clients_str())
        return True

    def quit_client(self, data):

        """Close connection with request source, remove from list of connected
        clients.

        """

        self.request.close()
        return self.server.del_client(data)

    def seperate_data(self, data, nsplit):

        """Breaks string data into tuple."""

        return tuple(re.split("\s+", data, nsplit))

    def seperate_header(self, data):

        """Returns header,message tuple."""

        return self.seperate_data(data, 1)

    def register_client(self, data):

        """Registers client with server."""

        client, ip, port = self.seperate_data(data, 2)
        return self.server.add_client(client, (ip, int(port)))

    def post(self, data):

        """Sends postcard data to listed clients."""

        source, clients_str, message = tuple(re.split(define.MSG_END, data, 2))
        clients = re.split("\s+", clients_str)

        for client in clients:
            self.server.clients_lock.acquire()
            try:
                if client in self.server.clients:
                    sock = socket.create_connection(self.server.clients[client])
                    sock.sendall(define.MSG_POST + " " + source + " " + message)
                    sock.close()
            finally:
                self.server.clients_lock.release()

    def private(self, data):

        """Sends private message data to listed client."""

        self.server.clients_lock.acquire()
        try:
            if data in self.server.clients:
                ip, port = self.server.clients[data]
                self.send_data(ip + " " + str(port))
            else:
                self.send_data(define.MSG_FAIL)
        finally:
            self.server.clients_lock.release()

    def reconcile(self, header, data, status):

        """Ensure executed action performed correctly. Clean-up."""

        if header == define.MSG_REG:
            if not status:
                self.send_data(define.MSG_FAIL)
                self.quit_client(data)
            else:
                self.send_data(define.MSG_OK)
        elif header == define.MSG_LIST:
            if not status:
                if not self.send_clients(data): # Try send one more time.
                    self.quit_client(data)
                else:
                    status = True
        elif header == define.MSG_POST:
            pass
        elif header == define.MSG_PRIV:
            pass
        elif header == define.MSG_QUIT:
            return False

        return status

    def process_data(self):

        """Processes data recieved from client."""

        status = True
        try:
            header, data = self.seperate_header(self.data)
        except:
            header = ""
            data = ""
            status = False

        if header == define.MSG_REG:
            status = self.register_client(data)
        elif header == define.MSG_LIST:
            status = self.send_clients(data)
        elif header == define.MSG_POST:
            self.post(data)
        elif header == define.MSG_PRIV:
            self.private(data)
        elif header == define.MSG_QUIT:
            status = self.quit_client(data)

        return self.reconcile(header, data, status)

    def handle(self):

        """Handles client connection."""

        stable = True
        while stable:
            self.get_data(4096)
            stable = self.process_data()

class Server(SocketServer.ThreadingMixIn, SocketServer.TCPServer):

    """Multithreaded server class."""

    def __init__(self, address, handler=RequestHandler):

        """Extends TCPServer constructor with additional data for the
        Server object:

        -Dictionary of clients: <client name> -> (<ip>, <port>)
                                (<string> -> (<string>, <int>))

        """

        SocketServer.TCPServer.__init__(self, address, handler)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.clients = {}
        self.clients_lock = threading.Lock()

    def add_client(self, name, address):

        """Add client name and address to client listing."""

        self.clients_lock.acquire()

        if name in self.clients:
            self.clients_lock.release()
            return False

        self.clients[name] = address
        self.clients_lock.release()
        return True

    def del_client(self, name):

        """Remove client name and address from client listing."""

        self.clients_lock.acquire()

        if name in self.clients:
            del self.clients[name]
            self.clients_lock.release()
            return True

        self.clients_lock.release()
        return False

    def host_str(self):

        """Returns host address as a raw string."""

        ip, port = self.server_address
        return ip + " " + str(port)

    def clients_str(self):

        return " ".join(self.clients.keys()).strip()

    def prompt(self, prompt):

        """Display prompt and wait for line of input from stdin."""

        sys.stdout.write(prompt)
        return sys.stdin.readline()

    def kill_all_clients(self):

        """Send kill message to all clients."""

        self.clients_lock.acquire()

        try:
            while len(self.clients):
                client_name, address = self.clients.popitem()
                sys.stdout.write("Closing " + client_name + "...\n")
                sys.stdout.flush()

                try:
                    sock = socket.create_connection(address)
                    sock.sendall(define.MSG_DOWN + " Server initiated shutdown...")
                    sock.close()
                except:
                    pass
        finally:
            self.clients_lock.release()

    def main_loop(self):

        """Main loop for getting shutdown command."""

        sys.stdout.write(self.host_str())

        loop = True

        try:
            while loop:
                command = self.prompt("\n>")
                command = command.strip()

                if(command == define.CMD_SHUT):
                    loop = False
                else:
                    sys.stdout.write("Unkown command\n")
                    sys.stdout.flush()
        except:
            raise
        finally:
            self.kill_all_clients()
            self.server_close()
            self.shutdown()
            
"""Logic for module execution starts here."""

if __name__ == "__main__":

    server = Server((socket.getfqdn(), 0))

    # Run two threads. One to handle new client connections, the other
    # for server input and display.
    remote_thread = threading.Thread(target=server.serve_forever)
    local_thread = threading.Thread(target=server.main_loop)

    remote_thread.setDaemon(True)
    remote_thread.start()
    local_thread.start()

    local_thread.join()
