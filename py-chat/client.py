"""TCP client module for CSCI 460 Assignment 1

Copyright (C) 2013  Tristan W. Bonsor

Multithreaded client module. Extends base classes provided by the SocketServer
module. Main connection to server is maintained until quit request is sent or
shutdown signal is recieved from server.

All output is sent through the client's listening socket, which spawns a new
thread that stores output and sends a signal to client's main loop, indicating
the display is ready to be updated. This allows for display of asynchronous
messages sent by other clients. Server and client initiated shutdown is handled
in a similar fashion.

"""

import curses
import random
import re
import select
import socket
import SocketServer
import sys
import threading

import define

class CursesUI:

    """User interface class that utilizes ncurses."""

    def __init__(self):

        """Starts curses and sets up terminal color and layout."""

        self.stdscr = curses.initscr()

        # Screen attributes.
        self.stdscr.nodelay(1) # Non-blocking user input.
        self.stdscr.keypad(1)
        curses.cbreak() # No line buffering, allow control signals.

        self.rows, self.cols = self.stdscr.getmaxyx()

        # Tracking user input and output.
        self.output_lines = []
        self.input_text = ""

        # Initialize colors.
        curses.start_color()

        # f => white, cyan, magenta, blue, yellow, green, red, black.
        # b => black, red, green, yellow, blue, magenta, cyan, white.
        # Thus, pair 0 is white on black, pair 8 is white on red, and so on.  
        n = 0
        for b in range(0, 8):
            for f in reversed(range(0, 8)):
                try:
                    curses.init_pair(n, f, b)
                except:
                    pass
                n += 1

        # Border output section.
        self.stdscr.attron(curses.color_pair(0) | curses.A_NORMAL)

        self.stdscr.hline(0, 1, curses.ACS_HLINE, self.cols - 2)
        self.stdscr.hline(self.rows - 2, 1, curses.ACS_HLINE, self.cols - 2)

        self.stdscr.vline(1, 0, curses.ACS_VLINE, self.rows - 3)
        self.stdscr.vline(1, self.cols - 1, curses.ACS_VLINE, self.rows - 3)

        self.stdscr.addch(0, 0, curses.ACS_ULCORNER)
        self.stdscr.addch(0, self.cols - 1, curses.ACS_URCORNER)
        self.stdscr.addch(self.rows - 2, 0, curses.ACS_LLCORNER)
        self.stdscr.addch(self.rows - 2, self.cols - 1, curses.ACS_LRCORNER)

        # Move curser to user prompt.
        self.stdscr.move(self.rows - 1, 0)

    def display_context(self, context):

        """Displays context centered on line above user input."""

        self.stdscr.attron(curses.color_pair(0) | curses.A_NORMAL)

        self.stdscr.hline(self.rows - 2, 1, curses.ACS_HLINE, self.cols - 2)
        self.stdscr.addch(self.rows - 2, 0, curses.ACS_LLCORNER)
        self.stdscr.addch(self.rows - 2, self.cols - 1, curses.ACS_LRCORNER)

        start_col = int(self.cols / 2) - int(len(context) / 2)

        self.stdscr.addstr(self.rows - 2, start_col, context, curses.A_REVERSE)

        self.stdscr.refresh()

    def display_title(self, title):

        """Displays title centered on top line."""

        self.stdscr.attron(curses.color_pair(0) | curses.A_NORMAL)

        self.stdscr.hline(0, 1, curses.ACS_HLINE, self.cols - 2)
        self.stdscr.addch(0, 0, curses.ACS_ULCORNER)
        self.stdscr.addch(0, self.cols - 1, curses.ACS_URCORNER)

        start_col = int(self.cols / 2) - int(len(title) / 2)

        self.stdscr.addstr(0, start_col, title, curses.A_REVERSE)

        self.stdscr.refresh()

    def user_input(self):

        """Adds or removes charcter from input string. Assumes getch is
        non-blocking. If enter key is pressed then input string is returned;
        the empty string is returned at all other times.

        """

        curses.noecho()
        ch = self.stdscr.getch()

        line = ""

        # Remove last character from input string
        if ch == curses.KEY_BACKSPACE:
            self.input_text = self.input_text[:-1]
        # Line of input is ready.
        elif ch == curses.KEY_ENTER or ch == 10:
            line = self.input_text
            self.input_text = ""
        # Otherwise append character of key press to string if possible.
        else:
            if len(self.input_text) == self.cols - 2:
                return line
            try:
                if chr(ch) != "\t": # Do not allow tabs (variable length).
                    self.input_text = self.input_text + chr(ch)
            except:
                pass

        # Display new output text at prompt.
        self.stdscr.addstr(self.rows - 1,
                           0,
                           self.input_text
                           + (" " * (self.cols - len(self.input_text) - 2)))

        # Move curser to end of input text.
        self.stdscr.move(self.rows - 1, len(self.input_text))
        self.stdscr.refresh()

        return line

    def add_output_line(self, line):

        """Appends line to output_lines list. Splits given line into mulitple
        lines if line length is longer than display width. Lines are appended
        to list with appropriate meta-data for user interface embellishments.

        """

        max_line_len = self.cols - 3

        context, output_line = tuple(re.split("\s", line, 1))

        # Add lines to output list. Line wrapping performed as necessary.
        new_lines = []

        if context == define.MSG_HELP:
            # Crude line wrapping.
            tmp_lines = re.split(r"[\n\r]+", output_line)
            for tmp_line in tmp_lines:
                for ix in range(0, len(tmp_line), max_line_len - 2):
                    new_lines.append(define.MSG_HELP + " "
                                     + tmp_line[ix:ix+max_line_len-2])
        elif context == define.MSG_LIST:
            # Fit as many intact client names on line.
            ix = 0
            while ix < len(output_line):
                new_line = output_line[ix:ix+max_line_len]
                if new_line[-1] != " ":
                    try:
                        overlap, rest = tuple(re.split("\s", new_line[::-1], 1))
                    except:
                        new_line = output_line
                    else:
                        new_line = output_line[ix:ix+max_line_len-len(overlap)]
                new_lines.append(define.MSG_LIST + " " + new_line.strip())
                ix += len(new_line)
        elif context == define.MSG_POST or context == define.MSG_PRIV:
            # Crude line wrapping. Prepend each line with sender's name.
            sender, message = tuple(re.split("\s", output_line, 1))
            ix = 0
            while ix < len(message):
                new_line = message[ix:ix+max_line_len-len(sender)-2]
                new_lines.append(context + " " + sender + " " + new_line)
                ix += len(new_line)
        elif context == define.MSG_DOWN:
            # No wrap necessary.
            new_lines.append(define.MSG_DOWN + " " + output_line)
        else:
            # Crude line wrapping.
            for ix in range(0, len(line), max_line_len):
                new_lines.append(line[ix:ix+max_line_len])

        # Add all new lines to list.
        self.output_lines.extend(new_lines)

    def display_output(self):

        """Displays contents of output_lines list from back to front. Lines are
        printed bottom to top up to display height.

        """

        # Clear displayed output.
        for row in reversed(range(1, self.rows - 2)):
            self.stdscr.addstr(row,
                               1,
                               (" " * (self.cols - 2)),
                               curses.color_pair(1))

        self.stdscr.refresh()

        # Display output from newest to oldest.
        row = self.rows - 3
        for line in reversed(self.output_lines):
            try:
                context, body = tuple(re.split("\s", line, 1))
            except:
                context = line

            # Print line with embellishments.
            if context == define.MSG_HELP:
                self.stdscr.addstr(row,
                                   1,
                                   ": " + body,
                                   curses.color_pair(3)| curses.A_BOLD)
            elif context == define.MSG_LIST:
                self.stdscr.addstr(row,
                                   1,
                                   body,
                                   curses.color_pair(1) | curses.A_BOLD)
            elif context == define.MSG_POST:
                sender, message = tuple(re.split("\s", body, 1))
                self.stdscr.addstr(row,
                                   1,
                                   sender,
                                   curses.color_pair(1) | curses.A_BOLD)
                self.stdscr.addstr(row,
                                   len(sender) + 1,
                                   ": " + message,
                                   curses.color_pair(0) | curses.A_NORMAL)
            elif context == define.MSG_PRIV:
                sender, message = tuple(re.split("\s", body, 1))
                self.stdscr.addstr(row,
                                   1,
                                   sender,
                                   curses.color_pair(1) | curses.A_BOLD)
                self.stdscr.addstr(row,
                                   len(sender) + 1,
                                   ": ",
                                   curses.color_pair(0) | curses.A_NORMAL)
                self.stdscr.addstr(row,
                                   len(sender) + 3,
                                   message,
                                   curses.color_pair(2) | curses.A_BOLD)
            elif context == define.MSG_DOWN:
                self.stdscr.addstr(row,
                                   1,
                                   body,
                                   curses.color_pair(6) | curses.A_BOLD)
            else:
                self.stdscr.addstr(row,
                                   1,
                                   line,
                                   curses.color_pair(4) | curses.A_BOLD)
            row -= 1
            if row == 0:
                break # Output screen filled.

        # Move curser to end of user input.
        self.stdscr.move(self.rows - 1, len(self.input_text))
        self.stdscr.refresh()

    def end_ui(self):

        """End curses terminal handling."""

        curses.endwin()

class RequestHandler(SocketServer.BaseRequestHandler):

    """Handler class for incoming requests to client."""

    def output_accept(self):

        """Accepts incoming output requests, assigning request to output_sig.
        output_sig notifies the main loop that there is new output to display.

        """

        self.server.output_sig, address = self.server.output_socket.accept()

    def shutdown_accept(self):

        """Accepts incoming shutdown requests, assigning request to shutdown_sig.
        shutdown_sig notifies the main loop that it is time to shutdown.

        """

        self.server.shutdown_sig, address = self.server.shutdown_socket.accept()

    def handle(self):

        """Handles each incoming request in its own thread. Since all requests
        will be output related, output lines are added to the output_line list
        and an output signal will be made.

        """

        self.data = self.request.recv(4096)
        head, body = tuple(re.split("\s", self.data, 1))

        # Add to output line list.
        self.server.output_lock.acquire()
        self.server.ui.add_output_line(self.data)

        # Start output acceptor in new thread.
        othread = threading.Thread(target=self.output_accept)
        othread.start()

        # Signal output acceptor.
        sock = socket.create_connection(self.server.output_address)
        sock.sendall(define.MSG_OK)
        sock.close()

        # Shutdown sequence engage.
        if head == define.MSG_DOWN:
            # Start shutdown acceptor in new thread.
            sthread = threading.Thread(target=self.shutdown_accept)
            sthread.start()

            # Signal shutdown acceptor.
            sock = socket.create_connection(self.server.shutdown_address)
            sock.sendall(define.MSG_DOWN)
            sock.close()

class Client(SocketServer.ThreadingMixIn, SocketServer.TCPServer):

    """Multithreaded client class."""

    def __init__(self, ui, host_address, client_name, handler=RequestHandler):

        """Connects to and registers with host, spawns asynchronous server for
        incoming messages, and initializes asynchronous signalling sockets for
        use with user input routines.

        If NameError is thrown then client name is already registered with host.

        """

        self.ui = ui
        self.output_lock = threading.Lock()

        self.host_address = host_address
        self.client_name = client_name

        self.host_socket = socket.create_connection(self.host_address)

        # Create threaded server to deal with asynchronous incoming data
        # from server and other clients.
        SocketServer.TCPServer.__init__(self, (socket.getfqdn(), 0), handler)

        # Register client name and listening address with host.
        listen_ip, listen_port = self.server_address
        self.host_socket.sendall(define.MSG_REG + " " + self.client_name + " "
                                 + listen_ip + " " + str(listen_port))


        if self.host_socket.recv(4096) == define.MSG_FAIL: # Client name in use.
            self.server_close()
            self.host_socket.close()
            raise NameError

        # Create two additional listening sockets for asynchronous signalling
        # while gathering user input.
        random.seed()

        while 1: # Socket for displaying asynchronous incoming messages.
            self.output_address = ("localhost", random.randint(1024, 65535))
            try:
                self.output_socket = socket.socket(socket.AF_INET,
                                                   socket.SOCK_STREAM)

                self.output_sig = self.output_socket

                self.output_socket.bind(self.output_address)
                self.output_socket.listen(1)
            except:
                pass # Port in use, try again.
            else:
                break # Socket ready and listening.

        while 1: # Socket for asynchronous client/server initiated shutdown.
            self.shutdown_address = ("localhost", random.randint(1024, 65535))
            try:
                self.shutdown_socket = socket.socket(socket.AF_INET,
                                                     socket.SOCK_STREAM)

                self.shutdown_sig = self.shutdown_socket

                self.shutdown_socket.bind(self.shutdown_address)
                self.shutdown_socket.listen(1)
            except:
                pass # Port in use, try again.
            else:
                break # Socket ready and listening.

    def process_user_input(self, data):

        """Recieves sanitized user input data as parameter. Performs actions
        based on meta-data tag at start of data.

        """

        # Send output request to display help information.
        if data == define.MSG_HELP:
            message = define.MSG_HELP + " "
            message += "help\n  Display help information.\n"
            message += "list\n  List names of all clients connected to server.\n"
            message += "postcard\n  Send a public message to one or multiple"
            message += " clients.\n"
            message += "privatemessage\n  Send a private message to one client.\n"
            message += "quit\n  De-register from server and terminate client.\n"

            sock = socket.create_connection(self.server_address)
            sock.sendall(message)
            sock.close()
            return

        # Get all connected clients from server and send output request to
        # display client list.
        elif data == define.MSG_LIST:
            self.host_socket.sendall(define.MSG_LIST + " " + self.client_name)
            response = self.host_socket.recv(4096)
            response.strip()

            sock = socket.create_connection(self.server_address)
            sock.sendall(define.MSG_LIST + " " + response)
            sock.close
            return

        # Notify server of client shutdown; send output request to display
        # user notification.
        elif data == define.MSG_QUIT:
            self.host_socket.sendall(define.MSG_QUIT + " " + self.client_name)

            sock = socket.create_connection(self.server_address)
            sock.sendall(define.MSG_DOWN + " Client initiated shutdown...")
            sock.close()
            return

        # Compound user input data.
        head, body = tuple(re.split("\s", data, 1))

        # Send postcard data to server. Send output request to display entire
        # command.
        if head == define.MSG_POST:
            self.host_socket.sendall(define.MSG_POST + " " + self.client_name
                                     + define.MSG_END + body)

            client_str, message = tuple(re.split(define.MSG_END, body, 1))

            sock = socket.create_connection(self.server_address)
            sock.sendall(define.CMD_POST + " " + client_str + " " + message)
            sock.close()

        # Get client address, send data to client, and send output request
        # to display entire command.
        elif head == define.MSG_PRIV:
            client_str, message = re.split(define.MSG_END, body, 1)
            client_list = re.split("\s", client_str)
            client = client_list[0]

            self.host_socket.sendall(define.MSG_PRIV + " " + client)
            response = self.host_socket.recv(4096)

            if response == define.MSG_FAIL: # Client not found on server.
                sock = socket.create_connection(self.server_address)
                sock.sendall(define.CMD_PRIV + " unknown client: " + client
                             + " " + message)
                sock.close()
                return

            ip, port = re.split("\s", response)

            sock = socket.create_connection((ip, int(port)))
            sock.sendall(define.MSG_PRIV + " " + self.client_name + " " + message)
            sock.close()

            sock = socket.create_connection(self.server_address)
            sock.sendall(define.CMD_PRIV + " " + client + " " + message)
            sock.close()

        # Unkown user input data.
        elif head == define.MSG_UNK:
            body = body.strip()

            if body != "":
                sock = socket.create_connection(self.server_address)
                sock.sendall("Unknown command: " + body)
                sock.close()

    def main_loop(self):

        """Provides main routine for client interaction with input and
        display. Sanitizes ready user input for further processing.
        Displays recieved data as it arrives asynchronously.

        """

        mode = 0 # Four input modes.
        context = ""
        client_str = ""

        run = True

        try:
            self.ui.display_title(self.client_name)

            ip, port = self.host_address
            sock = socket.create_connection(self.server_address)
            sock.sendall("Connected to " + ip + ":" + str(port))
            sock.close()

            self.ui.display_context("Enter command")

            while run:
                # Get any ready output or shutdown signals.
                rlist, wlist, xlist = select.select([self.shutdown_sig,
                                                     self.output_sig],
                                                    [],
                                                    [],
                                                    0.01)
                # Get line of user input if ready, otherwise empty
                # string (non-blocking).
                line = self.ui.user_input()

                # Process ready signals.
                for signal in rlist:
                    # Shutdown signal.
                    if signal == self.shutdown_sig:
                        run = False
                    # Output signal.
                    elif signal == self.output_sig:
                        self.ui.display_output()
                        try:
                            self.output_lock.release()
                        except:
                            pass
                        finally:
                            self.output_sig = self.output_socket

                # No shutdown signal and line of user input is ready.
                if run and line != "":
                    # Command mode.
                    if mode == 0:
                        # Sanitize user input.
                        line = line.strip()
                        line = line.lower()

                        # Find context of command and get next mode.
                        if line == define.CMD_HELP:
                            context = define.MSG_HELP
                            mode = 1
                        elif line == define.CMD_LIST:
                            context = define.MSG_LIST
                            mode = 1
                        elif line == define.CMD_POST:
                            mode = 2
                            context = define.MSG_POST
                            self.ui.display_context("Enter clients")
                        elif line == define.CMD_PRIV:
                            mode = 2
                            context = define.MSG_PRIV
                            self.ui.display_context("Enter client")
                        elif line == define.CMD_QUIT:
                            mode = 1
                            context = define.MSG_QUIT
                        else:
                            mode = 1
                            context = define.MSG_UNK + " " + line

                    # Get client(s) mode; context already gathered.
                    elif mode == 2:
                        client_str = line.strip()
                        mode = 3
                        self.ui.display_context("Enter message")
                    # Get message mode; context and client_str already
                    # gathered.
                    elif mode == 3:
                        self.process_user_input(context + " " + client_str
                                                + define.MSG_END + line)
                        mode = 0
                        self.ui.display_context("Enter command")

                    # One word or unknown command, always ready to process.
                    if mode == 1:
                        self.process_user_input(context)
                        mode = 0

        # Infinite loop broken by shutdown request. Clean up.
        finally:
            self.server_close()
            self.shutdown_sig.close()
            self.output_sig.close()
            self.shutdown_socket.close()
            self.output_socket.close()
            self.host_socket.sendall(define.MSG_QUIT + " " + self.client_name)
            self.host_socket.close()
            self.shutdown()
            self.ui.end_ui()

"""Logic for module execution starts here."""

if __name__ == "__main__":

    # Get command line args.
    try:

        host_ip, host_port, client_name = tuple(sys.argv[1:])

    except:

        sys.exit("Usage:\n\tclient.py host_ip host_port client_name\n"
                 + "Notes:\n\tclient_name must be one word")

    # Initialize user interface and client objects.
    try:

        ui = CursesUI()
        client = Client(ui, (host_ip, int(host_port)), client_name)

    except NameError:

        ui.end_ui()
        sys.exit("Client name already in use")

    except socket.gaierror:

        ui.end_ui()
        sys.exit("Host name or service unknown")

    except socket.error:

        ui.end_ui()
        sys.exit("Connection refused")

    except:
        curses.endwin()
        raise

    # Run two threads; one for incoming requests and the other
    # for user input and display.
    remote_thread = threading.Thread(target=client.serve_forever)
    local_thread = threading.Thread(target=client.main_loop)

    remote_thread.setDaemon(True)
    remote_thread.start()
    local_thread.start()

    local_thread.join()

    # Doubly make sure curses is done.
    try:
        curses.endwin()
    except:
        pass
