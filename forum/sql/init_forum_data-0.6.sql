/*******************************************************************************
 * File:    init_forum_data-0.6.sql
 * Created: Wed, Mar 28 at 22:30.53 PST
 * Author:  Tristan W. Bonsor 536 865 512; Craig Burnett 515 766 558
 * Version: 0.6
 *
 * Description: Slave script (run by init_forum-V.v.sql) 
 *		Inserts test data.
 ******************************************************************************/


INSERT INTO Forum
VALUES(1,'General Discussion','A forum for general discussion.');

INSERT INTO Forum
VALUES(2,'Math','Topics related to mathematics.');

INSERT INTO Forum
VALUES(3,'Programming','Topics related to programming.');

INSERT INTO Thread
VALUES(1,'Thread Math',0,FALSE,FALSE);

INSERT INTO Thread
VALUES(2,'Thead Programming',0,FALSE,FALSE);

INSERT INTO Thread
VALUES(3,'Hello World!',0,FALSE,FALSE);

INSERT INTO Thread
VALUES(4,'Tester',0,FALSE,FALSE);

INSERT INTO Contain
VALUES(2,1);

INSERT INTO Contain
VALUES(3,2);

INSERT INTO Contain
VALUES(1,3);

INSERT INTO Contain
VALUES(1,4);

INSERT INTO Message(creation_time, message_text)
VALUES('1992-01-01 00:00:01','Test test.');

INSERT INTO MadeOf
VALUES(4,5);

INSERT INTO Message(creation_time, message_text)
VALUES('1970-01-01 00:00:01','This is message 1.');

INSERT INTO MadeOf
VALUES(3,1);

INSERT INTO Message(creation_time, message_text)
VALUES('1983-01-31 03:12:59','This is message 2.');

INSERT INTO MadeOf
VALUES(2,2);

INSERT INTO Message(creation_time, message_text)
VALUES('1984-11-11 03:12:59','This is message 3.');

INSERT INTO MadeOf
VALUES(1,3);

INSERT INTO Message(creation_time, message_text)
VALUES('1983-03-31 03:12:59','This is message 4.');

INSERT INTO MadeOf
VALUES(1,4);

INSERT INTO Message(creation_time, message_text)
VALUES('1998-02-12 03:12:59','LA LA.');

INSERT INTO MadeOf
VALUES(3,6);

INSERT INTO User
VALUES(1,'admin','86f7e437faa5a7fce15d1ddcb9eaeaea377667b8','email','admin','istrator','1919-01-01','The Moon','1919-01-01',0,0,'');

INSERT INTO Administrator
VALUES(1,'');

INSERT INTO User
VALUES(2,'UserName1','a','b','c','d',1,'w',0,0,0,'');

INSERT INTO Start VALUES (1,1);
INSERT INTO Start VALUES (1,2);
INSERT INTO Start VALUES (1,3);
INSERT INTO Start VALUES (1,4);

INSERT INTO Post
VALUES(1,6);

INSERT INTO User
VALUES(3,'UserName0','a','b','c','d',1,'w',0,0,0,'');

INSERT INTO Post
VALUES(2,4);

INSERT INTO Moderator
VALUES(2,0);  
  
INSERT INTO MessageReports(user_id, message_id, isProcessed, timestamp, Reason)
VALUES(2, 6, 0, CURRENT_TIMESTAMP, 1);

INSERT INTO ThreadReports(user_id, thread_id, isProcessed, timestamp, Reason)
VALUES(2, 1, 0, CURRENT_TIMESTAMP, 1);


