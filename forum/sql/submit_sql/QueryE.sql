
-- E: For a specific user, how long has it been since they last logged in/ replied
SELECT User.user_name, 
	TIMESTAMPDIFF(MINUTE, MAX(User.logon_time), NOW()) As MinutesSinceLastLogon, 
	MAX(MessageLevelView.creation_time) As MostRecentReply 
FROM User 
	JOIN MessageLevelView ON User.user_id = MessageLevelView.user_id 
GROUP BY User.user_name 
HAVING User.user_name = 'derp'

/* Output: 
+-----------+-----------------------+---------------------+
| user_name | MinutesSinceLastLogon | MostRecentReply     |
+-----------+-----------------------+---------------------+
| derp      |                    95 | 2012-04-02 22:00:27 |
+-----------+-----------------------+---------------------+
1 row in set (0.00 sec)

*/

