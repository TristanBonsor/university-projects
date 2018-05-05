-- G: List the top 3 messages based on number of replies.
SELECT 
	parent_id as message_id, 
	COUNT(DISTINCT child_id) As num_replies  
FROM ChildOf  
WHERE parent_id = 1 and IFNULL(parent_id,0) <> 0 
GROUP BY parent_id ORDER BY num_replies DESC
--------------

/* 

+------------+-------------+
| message_id | num_replies |
+------------+-------------+
|          1 |           4 |
+------------+-------------+
1 row in set (0.00 sec)

*/
