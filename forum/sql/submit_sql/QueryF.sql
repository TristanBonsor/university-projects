-- F:  For each forum, how many messages (totalled over all threads in each forum) exist,
-- 	When was the most recent message?  When was the oldest and how many messages per day are posted?
--	Restricted to the top three busiest (messages per day) forums.

SELECT StatQry.*, 
	(SELECT CASE TIMESTAMPDIFF(DAY, OldestMsg, NewestMsg)
		WHEN 0 THEN StatQry.num_messages 
		ELSE StatQry.num_messages / TIMESTAMPDIFF(DAY, OldestMsg, NewestMsg) 
	END) AS MsgPerDay
FROM 
(SELECT 
	MessageLevelView.forum_name, 
	COUNT(DISTINCT MessageLevelView.message_id) as num_messages, 
	MAX(MessageLevelView.creation_time) AS NewestMsg, 
	MIN(MessageLevelView.creation_time) As OldestMsg
FROM MessageLevelView
GROUP BY MessageLevelView.forum_id) StatQry
ORDER BY MsgPerDay DESC LIMIT 3;

/* 

+--------------------+--------------+---------------------+---------------------+-----------+
| forum_name         | num_messages | NewestMsg           | OldestMsg           | MsgPerDay |
+--------------------+--------------+---------------------+---------------------+-----------+
| General Discussion |           10 | 2012-04-02 22:06:00 | 1983-03-31 03:12:59 |    0.0009 |
| Math               |            4 | 2012-04-02 22:00:27 | 1983-01-31 03:12:59 |    0.0004 |
| Programming        |            1 | NULL                | NULL                |      NULL |
+--------------------+--------------+---------------------+---------------------+-----------+
3 rows in set (0.00 sec)


*/


