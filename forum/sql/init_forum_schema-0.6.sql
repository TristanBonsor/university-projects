/*******************************************************************************
 * File:    init_forum_schema-0.6.sql
 * Created: RWed Mar 28 at 22:30.53 PST
 * Author:  Tristan W. Bonsor 536 865 512; Craig Burnett 515 766 558
 * Version: 0.6
 *
 * Description: Slave script (run by init_forum-V.v.sql)
 * 		Creates base tables and views.
 ******************************************************************************/

/* Entities */

/* Main table for users.  between this and the Settings table (not fully supported yet) all per-user information is stored. */
DROP TABLE IF EXISTS User;
CREATE TABLE User(
  user_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  user_name VARCHAR(32) NOT NULL UNIQUE,
  password VARCHAR(40) NOT NULL,
  email VARCHAR(32) NOT NULL,
  first_name VARCHAR(32),
  last_name VARCHAR(32),
  birthdate DATE NOT NULL,
  location VARCHAR(64),
  join_time TIMESTAMP NOT NULL,
  logon_time TIMESTAMP NULL,
  logon_count MEDIUMINT UNSIGNED NOT NULL DEFAULT 0,
  avatar_location VARCHAR(64)
);

DROP TABLE IF EXISTS Exceptions;
CREATE TABLE Exceptions (
	tableName VARCHAR(80),
	tstamp TIMESTAMP,
	tuple VARCHAR(10.5)
);


DROP TABLE IF EXISTS Moderator;
CREATE TABLE Moderator(
  moderator_id SMALLINT UNSIGNED NOT NULL,
  is_global BOOLEAN NOT NULL,
  FOREIGN KEY(moderator_id) REFERENCES User(user_id)
);

DROP TABLE IF EXISTS Administrator;
CREATE TABLE Administrator(
  admin_id SMALLINT UNSIGNED NOT NULL,
  admin_password VARCHAR(32) NOT NULL,
  FOREIGN KEY(admin_id) REFERENCES Moderator(moderator_id)
);

DROP TABLE IF EXISTS Setting;
CREATE TABLE Setting(
  setting_id SMALLINT UNSIGNED NOT NULL,
  display_mode TINYINT UNSIGNED NOT NULL,
  show_lastpost BOOLEAN NOT NULL,
  show_threadcount BOOLEAN NOT NULL,
  show_postcount BOOLEAN NOT NULL,
  show_viewcount BOOLEAN NOT NULL,
  show_avatars BOOLEAN NOT NULL,
  FOREIGN KEY(setting_id) REFERENCES User(user_id)
);

DROP TABLE IF EXISTS Message_base;
CREATE TABLE Message_base(
  message_id MEDIUMINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  creation_time TIMESTAMP NOT NULL,
  message_text TEXT NOT NULL
);
/* Don't allow messages from the future or with null timestamp! */
CREATE OR REPLACE VIEW Message AS
	SELECT * FROM Message_base WHERE TIMESTAMPDIFF(SECOND, creation_time, NOW()) >= 0
WITH CHECK OPTION;

DROP TABLE IF EXISTS Thread;
CREATE TABLE Thread(
  thread_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  thread_name VARCHAR(64) NOT NULL UNIQUE,
  view_count MEDIUMINT UNSIGNED NOT NULL,
  is_locked BOOLEAN NOT NULL,
  is_sticky BOOLEAN NOT NULL
);

DROP TABLE IF EXISTS Forum;
CREATE TABLE Forum(
  forum_id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  forum_name VARCHAR(32) NOT NULL UNIQUE,
  description VARCHAR(64) NOT NULL
);

/* Relations */

DROP TABLE IF EXISTS Post;
CREATE TABLE Post(
  user_id SMALLINT UNSIGNED NOT NULL,
  message_id MEDIUMINT UNSIGNED NOT NULL,
  FOREIGN KEY(user_id) REFERENCES User(user_id),
  FOREIGN KEY(message_id) REFERENCES Message(message_id)
);

DROP TABLE IF EXISTS Start;
CREATE TABLE Start(
  user_id SMALLINT UNSIGNED NOT NULL,
  thread_id SMALLINT UNSIGNED NOT NULL,
  FOREIGN KEY(user_id) REFERENCES User(user_id),
  FOREIGN KEY(thread_id) REFERENCES Thread(thread_id)
);

DROP TABLE IF EXISTS BlackList_base;
CREATE TABLE BlackList_base(
  lister_id SMALLINT UNSIGNED NOT NULL,
  listee_id SMALLINT UNSIGNED NOT NULL,
  FOREIGN KEY(lister_id) REFERENCES User(user_id),
  FOREIGN KEY(listee_id) REFERENCES User(user_id)
);
/* Don't allow users to blacklist themselves?! */
CREATE OR REPLACE VIEW BlackList AS
	SELECT * FROM BlackList_base WHERE lister_id <> listee_id
WITH CHECK OPTION;


DROP TABLE IF EXISTS Suspend;
CREATE TABLE Suspend(
  moderator_id SMALLINT UNSIGNED NOT NULL,
  user_id SMALLINT UNSIGNED NOT NULL,
  begin_time TIMESTAMP NOT NULL,
  end_time TIMESTAMP NOT NULL,
  reason VARCHAR(64),
  FOREIGN KEY(moderator_id) REFERENCES Moderator(moderator_id),
  FOREIGN KEY(user_id) REFERENCES User(user_id)
);

DROP TABLE IF EXISTS Moderate;
CREATE TABLE Moderate(
  moderator_id SMALLINT UNSIGNED NOT NULL,
  forum_id TINYINT UNSIGNED NOT NULL,
  authority_level TINYINT UNSIGNED NOT NULL,
  FOREIGN KEY(moderator_id) REFERENCES Moderator(moderator_id),
  FOREIGN KEY(forum_id) REFERENCES Forum(forum_id)
);

DROP TABLE IF EXISTS Review;
CREATE TABLE Review(
  moderator_id SMALLINT UNSIGNED NOT NULL,
  message_id MEDIUMINT UNSIGNED NOT NULL,
  reason VARCHAR(64),
  FOREIGN KEY(moderator_id) REFERENCES Moderator(moderator_id),
  FOREIGN KEY(message_id) REFERENCES Message(message_id)
);

DROP TABLE IF EXISTS Move_base;
CREATE TABLE Move_base(
  moderator_id SMALLINT UNSIGNED NOT NULL,
  thread_id SMALLINT UNSIGNED NOT NULL,
  forum_from SMALLINT UNSIGNED NOT NULL,
  forum_to SMALLINT UNSIGNED NOT NULL,
  reason VARCHAR(64),
  FOREIGN KEY(moderator_id) REFERENCES Moderator(moderator_id),
  FOREIGN KEY(thread_id) REFERENCES Thread(thread_id)
);
/* Don't allow threads to be moved from a forum to the same forum */
CREATE OR REPLACE VIEW Move AS
	SELECT * FROM Move_base WHERE forum_from <> forum_to
WITH CHECK OPTION;


DROP TABLE IF EXISTS ChildOf_base;
CREATE TABLE ChildOf_base(
  child_id MEDIUMINT UNSIGNED NOT NULL,
  parent_id MEDIUMINT UNSIGNED NOT NULL,
  FOREIGN KEY(child_id) REFERENCES Message(message_id),
  FOREIGN KEY(parent_id) REFERENCES Message(message_id)
);
/* Don't allow a message to be its own parent */
CREATE OR REPLACE VIEW ChildOf AS
	SELECT * FROM ChildOf_base WHERE child_id <> parent_id
WITH CHECK OPTION;


DROP TABLE IF EXISTS MadeOf;
CREATE TABLE MadeOf(
  thread_id SMALLINT UNSIGNED NOT NULL,
  message_id MEDIUMINT UNSIGNED NOT NULL,
  FOREIGN KEY(thread_id) REFERENCES Thread(thread_id),
  FOREIGN KEY(message_id) REFERENCES Message(message_id)
);

DROP TABLE IF EXISTS Contain;
CREATE TABLE Contain(
  forum_id TINYINT UNSIGNED NOT NULL,
  thread_id SMALLINT UNSIGNED NOT NULL,
  FOREIGN KEY(forum_id) REFERENCES Forum(forum_id),
  FOREIGN KEY(thread_id) REFERENCES Thread(thread_id)
);

DROP TABLE IF EXISTS MessageReports;
CREATE TABLE MessageReports(
  user_id SMALLINT UNSIGNED NOT NULL,
  message_id MEDIUMINT UNSIGNED NOT NULL,
  isProcessed int NOT NULL DEFAULT '0',
  timestamp TIMESTAMP NOT NULL,
  Reason int NOT NULL,
  FOREIGN KEY(user_id) REFERENCES User(user_id),
  FOREIGN KEY(message_id) REFERENCES Message(message_id)
);

DROP TABLE IF EXISTS ThreadReports;
CREATE TABLE ThreadReports(
  user_id SMALLINT UNSIGNED NOT NULL,
  thread_id MEDIUMINT UNSIGNED NOT NULL,
  isProcessed int NOT NULL DEFAULT '0',
  timestamp TIMESTAMP NOT NULL,
  Reason int NOT NULL,
  FOREIGN KEY(user_id) REFERENCES User(user_id),
  FOREIGN KEY(thread_id) REFERENCES Thread(thread_id)
);


CREATE OR REPLACE VIEW ThreadReplyCount AS
SELECT
  thread_id,
  COUNT(message_id) - 1 AS reply_count
FROM
  MadeOf
  NATURAL LEFT OUTER JOIN Message
GROUP BY
  thread_id;

CREATE OR REPLACE VIEW ThreadCreated AS
SELECT
  thread_id,
  MIN(creation_time) AS creation_time
FROM
  MadeOf
  NATURAL LEFT OUTER JOIN Message
GROUP BY
  thread_id;

CREATE OR REPLACE VIEW ThreadUpdated AS
SELECT
  thread_id,
  MAX(creation_time) AS update_time
FROM
  MadeOf
  NATURAL LEFT OUTER JOIN Message
GROUP BY
  thread_id;

CREATE OR REPLACE VIEW ThreadCreatedWith AS
SELECT
  thread_id,
  creation_time,
  message_id
FROM
  ThreadCreated
  NATURAL LEFT OUTER JOIN Message;

CREATE OR REPLACE VIEW ThreadUpdatedWith AS
SELECT
  thread_id,
  update_time,
  message_id
FROM
  ThreadUpdated
  NATURAL LEFT OUTER JOIN Message
WHERE
  update_time = creation_time;

CREATE OR REPLACE VIEW ThreadCreatedBy AS
SELECT
  thread_id,
  creation_time,
  user_id,
  user_name
FROM
  ThreadCreatedWith
  NATURAL LEFT OUTER JOIN Post
  NATURAL LEFT OUTER JOIN User;

CREATE OR REPLACE VIEW ThreadUpdatedBy AS
SELECT
  thread_id,
  update_time,
  user_id,
  user_name
FROM
  ThreadUpdatedWith
  NATURAL LEFT OUTER JOIN Post
  NATURAL LEFT OUTER JOIN User;

CREATE OR REPLACE VIEW ThreadCreatedView AS
SELECT
  thread_id,
  message_id,
  creation_time,
  user_id,
  user_name
FROM
  ThreadCreatedWith
  NATURAL LEFT OUTER JOIN ThreadCreatedBy;

CREATE OR REPLACE VIEW ThreadUpdatedView AS
SELECT
  thread_id,
  message_id,
  update_time,
  user_id,
  user_name
FROM
  ThreadUpdatedWith
  NATURAL LEFT OUTER JOIN ThreadUpdatedBy;

CREATE OR REPLACE VIEW ThreadLevelView AS
SELECT
  Contain.forum_id,
  t5.forum_name,
  t3.thread_id,
  t3.thread_name,
  t1.message_id AS original_message_id,
  t1.creation_time,
  t1.user_id AS original_user_id,
  t1.user_name AS original_user_name,
  t2.message_id AS latest_message_id,
  t2.update_time AS update_time,
  t2.user_id AS latest_user_id,
  t2.user_name AS latest_user_name,
  t4.reply_count,
  t3.view_count
FROM
  Contain
  NATURAL LEFT OUTER JOIN ThreadCreatedView AS t1
  JOIN ThreadUpdatedView AS t2
  NATURAL LEFT OUTER JOIN Thread AS t3
  NATURAL LEFT OUTER JOIN ThreadReplyCount AS t4
  NATURAL LEFT OUTER JOIN Forum As t5
WHERE
  t1.thread_id = t2.thread_id
	and Contain.forum_id = t5.forum_id
ORDER BY
  update_time DESC;

CREATE OR REPLACE VIEW ForumThreadCount AS
SELECT
  forum_id,
  COUNT(thread_id) AS thread_count
FROM
  Contain
GROUP BY
  forum_id;

CREATE OR REPLACE VIEW ForumMessageCount AS
SELECT
  forum_id,
  COUNT(message_id) AS message_count
FROM
  Contain
  NATURAL LEFT OUTER JOIN Thread
  NATURAL LEFT OUTER JOIN MadeOf
GROUP BY
  forum_id;

CREATE OR REPLACE VIEW ForumUpdatedView AS
SELECT
  forum_id,
  MAX(latest_message_id) AS latest_message_id,
  MAX(update_time) AS update_time
FROM
  ThreadLevelView
GROUP BY
  forum_id;

CREATE OR REPLACE VIEW ForumLevelView AS
SELECT
  forum_id,
  forum_name,
  description,
  thread_id,
  thread_name,
  latest_message_id,
  update_time,
  latest_user_id,
  latest_user_name,
  thread_count,
  message_count
FROM
  Forum
  NATURAL LEFT OUTER JOIN ThreadLevelView
  NATURAL LEFT OUTER JOIN ForumThreadCount
  NATURAL LEFT OUTER JOIN ForumMessageCount
ORDER BY
  forum_id;

CREATE OR REPLACE VIEW MessageLevelView AS
SELECT
  Forum.forum_id,
  Forum.forum_name,
  thread_id,
  thread_name, 
  message_id,
  creation_time,
  message_text,
  user_id,
  user_name,
  location,
  join_time,
  avatar_location
FROM
  Contain
  NATURAL LEFT OUTER JOIN MadeOf
  NATURAL LEFT OUTER JOIN Message
  NATURAL LEFT OUTER JOIN Post
  NATURAL LEFT OUTER JOIN User
  NATURAL LEFT OUTER JOIN Thread
  NATURAL LEFT OUTER JOIN Forum
WHERE 
	Contain.forum_id = Forum.forum_id
ORDER BY
  creation_time;


CREATE OR REPLACE VIEW TopLevelView AS
SELECT
  *
FROM
  ForumUpdatedView
  NATURAL LEFT OUTER JOIN ForumLevelView;

CREATE OR REPLACE VIEW ModeratorQueue AS 
	(SELECT 
		1 AS ReportType, 
		R.user_id AS ReportedBy,
		(SELECT user_name FROM User WHERE user_id = R.user_id LIMIT 1) As ReportedByUserName,
		U.user_id AS OffendingUser,
		(SELECT user_name FROM User WHERE user_id = U.user_id LIMIT 1) As OffendingUserName,
		R.timestamp,
		R.Reason,
		M.message_id AS ObjectID,
		M.message_text AS Text
        FROM
		MessageReports R INNER JOIN Message AS M INNER JOIN Post AS U
	WHERE 
		R.isProcessed = 0 and R.message_id = M.message_id and M.message_id = U.message_id
	ORDER BY timestamp ASC)
	UNION ALL
	(SELECT
		2 AS ReportType,
		R.user_id AS ReportedBy,
		(SELECT user_name FROM User WHERE user_id = R.user_id LIMIT 1) As ReportedByUserName,
		U.user_id AS OffendingUser,
		(SELECT user_name FROM User WHERE user_id = U.user_id LIMIT 1) As OffendingUserName,
		R.timestamp,
		R.Reason,
		T.thread_id AS ObjectID,
		T.thread_name As Text
	 FROM
		ThreadReports AS R INNER JOIN Thread AS T INNER JOIN Start AS U
	WHERE R.isProcessed = 0 and R.thread_id = T.thread_id and T.thread_id = U.thread_id
	ORDER BY timestamp ASC)
;


