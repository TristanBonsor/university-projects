/*******************************************************************************
 * File:    init_forum_procedures-0.6.txt
 * Created: Wed, Mar 28 at 22:30.53 PST
 * Author:  Tristan W. Bonsor, 536 865 512; Craig Burnett 515 766 558
 * Version: 0.6
 *
 * Description: Slave script (run by init_forum-V.v.sql)
 * 		Creates All Functions and Procedures
 ******************************************************************************/

DELIMITER $$

/* BEGIN Dr. J.'s Spectacular Workaround */
DROP PROCEDURE IF EXISTS raise_application_error$$
DROP PROCEDURE IF EXISTS get_last_custom_error$$
DROP TABLE IF EXISTS RAISE_ERROR$$


CREATE PROCEDURE raise_application_error(IN CODE INTEGER, IN MESSAGE VARCHAR(255)) SQL SECURITY INVOKER DETERMINISTIC
BEGIN
	CREATE TEMPORARY TABLE IF NOT EXISTS RAISE_ERROR(F1 INT NOT NULL);
	SELECT CODE, MESSAGE INTO @error_code, @error_message;
	INSERT INTO RAISE_ERROR VALUES(NULL);
END
$$

CREATE PROCEDURE get_last_custom_error() SQL SECURITY INVOKER DETERMINISTIC
BEGIN
  SELECT @error_code, @error_message;
END;
$$
/* End Stolen Code */

DROP FUNCTION IF EXISTS IsBlank$$
CREATE FUNCTION IsBlank (_val VARCHAR(65535)) RETURNS INTEGER
BEGIN
	IF _val IS NULL or length(_val) = 0 THEN
		RETURN 1;
	END IF;
	RETURN 0;
END$$

DROP PROCEDURE IF EXISTS AddUser$$
CREATE PROCEDURE AddUser (
	_UserName VARCHAR(32), 
	_PwdHash VARCHAR(40), 
	_email VARCHAR(32), 
	_fname VARCHAR(32),
	_lname VARCHAR(32),
	_birthdate DATE,
	_location VARCHAR(64),
	_avatar VARCHAR(64)) 
BEGIN
	DECLARE NowTime TIMESTAMP;
	SELECT NOW() INTO NowTime;
/* handled by insert trigger now
	IF IsBlank(_UserName) THEN 		
		RETURN 'user_name:Please Enter a Username';
	ELSEIF EXISTS(SELECT user_id FROM User WHERE user_name = _UserName) THEN
		RETURN 'user_name:That user name is already in use.  Please choose another.'; 
	END IF;

	IF IsBlank(_PwdHash) THEN
		RETURN 'password:Please Enter a Password';
	END IF;
	IF IsBlank(_email) THEN
		RETURN 'email:Please enter an email address.';
	END IF;
*/
	INSERT INTO User (user_name, password, email, first_name, last_name, birthdate, location, join_time, logon_time, avatar_location)
	VALUES(_UserName, _PwdHash, _email, _fname, _lname, _birthdate, _location, NowTime, NULL, _avatar);
END$$

DROP FUNCTION IF EXISTS CurrentSessionID$$
CREATE FUNCTION CurrentSessionID (_user_id INTEGER, _session_id INTEGER) RETURNS INTEGER 
BEGIN
	SELECT 
		session_id INTO @_session_id 
	FROM 
		UserSession 
	WHERE 
		user_id = @_user_id 
		AND CURRENT_TIMESTAMP BETWEEN SessionCreated AND SessionExpires;
	IF @_session_id IS NULL THEN
		RETURN 0;
	END IF;
	UPDATE UserSession SET SessionExpires = TIMESTAMP_ADD(MINUTE, 120, CURRENT_TIMESTAMP) WHERE session_id = @_session_id;
	RETURN 1;
END$$
 

DROP PROCEDURE IF EXISTS NewSession$$
CREATE PROCEDURE NewSession(IN _user_id INTEGER, OUT _session_id VARCHAR(16))
BEGIN
	
	SELECT MD5(Rand()) INTO @_session_id;
	WHILE EXISTS(SELECT session_id FROM UserSession WHERE session_id like @_session_id LIMIT 1) DO
		SELECT MD5(Rand()) INTO @_session_d;
	END WHILE;

	/* @_session_id isnow a unique alphanumeric string w.r.t. UserSession.session_id */

	INSERT INTO UserSession (session_id, user_id, SessionCreated, SessionExpires)
		VALUES (@_session_id, @_user_id, CURRENT_TIMESTAMP, TIMESTAMP_ADD(MINUTE,120,CURRENT_TIMESTAMP));
END$$	

DROP PROCEDURE IF EXISTS UserLogin$$
CREATE PROCEDURE UserLogin(IN _user_name VARCHAR(32), IN _pwdHash VARCHAR(32), 
			   OUT _session_id INTEGER, OUT _PreviousLogin TIMESTAMP, OUT _login_error VARCHAR(80))
BEGIN
	DECLARE user_id INTEGER;

	IF !EXISTS(SELECT LastLogin, user_id  INTO @_PreviousLogin, @user_id FROM User WHERE user_name = @_user_name AND password = @_pwdHash) THEN
		SET @login_error = 'Invalid user name or password';
	ELSE
		IF (!CurrentSessionID(@user_id, @_session_id)) THEN
			CALL NewSession(user_id, @session_id);
		END IF;
	END IF;
END$$


DROP FUNCTION IF EXISTS IsLoggedIn$$
CREATE FUNCTION IsLoggedIn (_user_id SMALLINT, _session_id SMALLINT) RETURNS INTEGER
BEGIN
	RETURN EXISTS (SELECT * FROM User WHERE user_id = _user_id AND session_id = _session_id);
END$$


/*
DROP PROCEDURE IF EXISTS Logout
CREATE PROCEDURE Logout(IN u_id SMALLINT, IN s_id SMALLINT)
*/	


DROP PROCEDURE IF EXISTS postReply
$$
CREATE PROCEDURE postReply(threadID SMALLINT,
       		  	   parentID MEDIUMINT,
       		           userID SMALLINT,
			   message TEXT)
BEGIN
  INSERT INTO Message (creation_time,message_text)
  VALUES (NOW(),message);
  SET @messageID = LAST_INSERT_ID();
  INSERT INTO MadeOf VALUES (threadID,@messageID);
  INSERT INTO ChildOf VALUES (@messageID,parentID);
  INSERT INTO Post VALUES (userID,@messageID);
END
$$


DROP PROCEDURE IF EXISTS createThread
$$
CREATE PROCEDURE createThread(forumID TINYINT,
			      threadName VARCHAR(64),
			      userID SMALLINT,
			      message TEXT)
BEGIN
  INSERT INTO Thread (thread_name,view_count,is_locked,is_sticky)
  VALUES (threadName,0,FALSE,FALSE);
  SET @threadID = LAST_INSERT_ID();
  INSERT INTO Contain VALUES (forumID,@threadID);
  INSERT INTO Message (creation_time,message_text)
  VALUES (NOW(),message);
  SET @messageID = LAST_INSERT_ID();
  INSERT INTO MadeOf VALUES (@threadID,@messageID);
  INSERT INTO ChildOf VALUES (@messageID,0);
  INSERT INTO Post VALUES (userID,@messageID);
  INSERT INTO Start VALUES(userID,@threadID);
END
$$

DELIMITER ;


