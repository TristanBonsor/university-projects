-- H: List the top 3 contrbuting users based on frequency of messages.
SELECT 
	CONCAT(User.last_name, ', ', User.first_name, ' (', User.user_name, ')') As user_description,
	User.join_time as user_since, 
	StatQry.num_msg as total_message_count,
	(SELECT CASE StatQry.TimeSpan WHEN 0 THEN 0 ELSE StatQry.num_msg / StatQry.TimeSpan END) As messages_per_hour
FROM User NATURAL LEFT OUTER JOIN 
	(SELECT 
		user_id, 
		IFNULL(TIMESTAMPDIFF(HOUR, MIN(creation_time),MAX(creation_time)),0) As TimeSpan, 
		COUNT(message_id) as num_msg
	FROM MessageLevelView
	GROUP BY user_id) StatQry
ORDER BY messages_per_hour DESC, total_message_count DESC LIMIT 3;


/* 

This could be used to find bots who are posting a little too often or to find users who
regularly contribute content so we can give them candy.

+---------------------------+---------------------+---------------------+-------------------+
| user_description          | user_since          | total_message_count | messages_per_hour |
+---------------------------+---------------------+---------------------+-------------------+
| wil_last, wil_first (wil) | 2012-04-02 23:42:07 |                   7 |            7.0000 |
| derpl, derpf (derp)       | 2012-04-02 23:39:39 |                  11 |            2.7500 |
| d, c (UserName0)          | 0000-00-00 00:00:00 |                   1 |            0.0000 |
+---------------------------+---------------------+---------------------+-------------------+

*/


