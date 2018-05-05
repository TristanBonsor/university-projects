/*******************************************************************************
 * File:    init_forum-0.6.sql
 * Created: Wed, Mar 28 at 13:30.53 PST
 * Author:  Tristan W. Bonsor 536 865 512; Craig Burnett 515 766 558
 * Version: 0.6
 *
 * Description: This is the master initialization script for the whole project. 
 * 		It destroys and recreates all objects and data from scratch
 ******************************************************************************/


/* This needs to be removed at school because we lack CREATE, DROP at database level
*/
/*use burnetcr;
/*
end non-school-friendly section */

source ./init_forum_schema-0.6.sql;
show errors;
show warnings;

source ./init_forum_triggers-0.6.sql;
show errors;
show warnings;

source ./init_forum_constraints-0.6.sql;
show errors;
show warnings;

source ./init_forum_procedures-0.6.sql;
show errors;
show warnings;

source ./init_forum_data-0.6.sql;
show errors;
show warnings;

