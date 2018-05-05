
-- B: For a specific forum, a list of all threads in that forum
SELECT DISTINCT thread_name FROM ForumLevelView WHERE forum_name = 'General Discussion' ORDER BY thread_name;

/* Sample output: 
+--------------+
| thread_name  |
+--------------+
| Hello World! |
| Tester       |
+--------------+
2 rows in set (0.00 sec)

Too simple?  The ForumLevelView looks like this:

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

*/


