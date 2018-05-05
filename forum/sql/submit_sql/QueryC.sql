
--  C: For a specific message (message_id=1 in this example), how many different users have replied to it?
select CO.parent_id, COUNT(DISTINCT MLV.user_id) as num_repliers 
FROM ChildOf CO 
	JOIN MessageLevelView MLV ON CO.child_id = MLV.message_id 
WHERE CO.parent_id = 1 
GROUP BY CO.parent_id

/* 
+-----------+--------------+
| parent_id | num_repliers |
+-----------+--------------+
|         1 |            2 |
+-----------+--------------+
1 row in set (0.00 sec)

In our database, 'derp' and 'wil' have replied to this message.

*/


