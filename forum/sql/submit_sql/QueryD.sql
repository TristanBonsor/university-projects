
-- For fun and challenge, how many messages, messages will we likely have by some future date if we keep growing at the current rate (assuming linearity)?
SELECT 
	(SELECT CASE IFNULL(days,0) 
	 WHEN 0 THEN num_msg 
	 ELSE num_msg / days 
	END) * TIMESTAMPDIFF(DAY, earliest, '2013-01-01') As projected_messages 
FROM 
(SELECT TIMESTAMPDIFF(DAY, MIN(creation_time), MAX(creation_time)) As days, 
	MIN(creation_time) as earliest, 
	COUNT(message_id) As num_msg 
	FROM Message) sqry

/* 

The subquery gets the difference in days between the timestamps on the earliest and latest messages as well as the number of messages created during that time.
The outer query then divides the number of messages by the number of days and multiplies by the number of days from the earliest message to the projected date (currently hard-coded.  Would be implemented as a parameter to a stored procedure or function).  

In our test database, we have some messages from the 1970's and very vew messages overall so this 
is actually a correct result.  

+--------------------+
| projected_messages |
+--------------------+
|            14.3587 |
+--------------------+
1 row in set (0.00 sec)

*/

