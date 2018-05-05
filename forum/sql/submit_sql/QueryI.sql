-- I List the top three user_id's based on overall contribution to the site
SELECT user_id, COUNT(DISTINCT message_id) + COUNT(DISTINCT thread_id) * 10 As TotalContribution 
FROM Post 
	NATURAL LEFT OUTER JOIN Start 
GROUP BY user_id ORDER BY TotalContribution DESC
--------------

/* 

A user 'Start's a thread or 'Post's a message.
We will give a score of 10 for each thread started and a score of 1 for each message posted.

Results:
+---------+-------------------+
| user_id | TotalContribution |
+---------+-------------------+
|       1 |                41 |
|       3 |                11 |
|       4 |                 7 |
|       2 |                 1 |
+---------+-------------------+

*/


