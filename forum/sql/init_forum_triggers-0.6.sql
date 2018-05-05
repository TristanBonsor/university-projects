/*******************************************************************************
 * File:    init_forum_triggers-0.6.sql
 * Created: Wed, Mar 28 at 13:30.53 PST
 * Author:  Tristan W. Bonsor 536 865 512; Craig Burnett 515 766 558
 * Version: 0.6
 *
 * Description: Slave script (run by init_forum-V.v.sql) 
 *		Creates triggers.
 ******************************************************************************/


DELIMITER $$


/* trigger to disallow duplicate usernames, blank passwords, blank email addresses or birthdates in the future. */
/* Builds a nicely formatted "field1:error1\nfield2:error2\n... field_k: error_k" string. and calls raise_application_error */
DROP TRIGGER IF EXISTS trgUser_Insert$$
CREATE TRIGGER trgUser_Insert BEFORE INSERT ON User FOR EACH ROW
BEGIN
	DECLARE Err VARCHAR(1024);

	DECLARE NowTime TIMESTAMP;
	SELECT NOW() INTO NowTime;

	SET Err = '';	
	IF exists(SELECT user_name FROM User WHERE user_name LIKE NEW.user_name LIMIT 1) THEN	
		SET Err = CONCAT(Err, "user_name:That user name already exists.  Please select another.\n");
	ELSEIF IsBlank(NEW.user_name) THEN
		SET Err = CONCAT(Err, "user_name:Please enter a user name.\n");
	END IF;

	IF IsBlank(NEW.password) THEN
		SET Err = CONCAT(Err, "password:Please enter a password.\n");
	END IF;

	IF IsBlank(NEW.email) THEN
		SET Err = CONCAT(Err, "email:Please ener an email.\n");
	END IF;
	IF NEW.birthdate IS NOT NULL AND TIMESTAMPDIFF(SECOND, NEW.birthdate, NowTime) < 0 THEN
		SET Err = CONCAT(Err, "birthdate:Please enter a date in the past.\n");
	END IF;
/*
	SET NEW.join_time = NOW();
*/
/* logon_count is unsigned.
	If NEW.logon_count < 0 THEN
		SET Err = CONCAT(Err, "logon_count:Must not be negative\n");
	END IF;
*/
	IF LENGTH(Err) > 0 THEN
		CALL raise_application_error(-911, Err);
	END IF;
END;
$$

/* When a user logs in, the application manually increments the logon count.  This trigger stores the time when that happens 
 in user.logon_rime */
DROP TRIGGER IF EXISTS trgUser_Update$$
CREATE TRIGGER trgUser_Update BEFORE UPDATE ON User FOR EACH ROW
BEGIN
	IF NEW.logon_count > OLD.logon_count THEN
		SET NEW.logon_time = NOW();
	END IF;
END;
$$

/* When a new reply message is created as a child of another message in the ChildOf table,
 this trigger fires and ensures that the parent actually exists, is in a thread (MadeOf table)
 and then adds the child message to the same thread for us. */
DROP TRIGGER IF EXISTS trgChildOf_Update$$
CREATE TRIGGER trgChildOf_Update BEFORE INSERT ON ChildOf_base FOR EACH ROW
BEGIN
	DECLARE thr_id SMALLINT;

	IF NOT EXISTS (SELECT thread_id INTO thr_id FROM MadeOf WHERE message_id = NEW.parent_id) THEN
		CALL raise_application_error(-1, 'ChildOf: Parent message not in a thread');
	END IF;
	IF NOT EXISTS (SELECT * FROM MadeOf WHERE message_id = NEW.child_id) THEN
		INSERT INTO MadeOf (message_id, thread_id) VALUES (NEW.child_id, thr_id);
	END IF;
END;
$$


DELIMITER ;


