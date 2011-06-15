set foreign_key_checks  = 0;

drop table if exists users;

create table users (
       id INT NOT NULL AUTO_INCREMENT,
       primary key (id),
       name varchar(50) not null
) engine=innodb;

drop table if exists cartoons;

create table cartoons (
       id INT NOT NULL AUTO_INCREMENT,
       primary key (id),
       name varchar(100) not null,
       season INT NOT NULL,
       episode INT NOT NULL,
       title varchar(100) not null 
) engine=innodb;

drop table if exists cartoons_owners;

create table cartoons_owners (
       file_id INT NOT NULL,
       foreign key (file_id) references cartoons(id)
       on delete cascade,
       owner_id INT NOT NULL,
       foreign key (owner_id) references users(id)
       on delete cascade
) engine=innodb;

set foreign_key_checks  = 1;