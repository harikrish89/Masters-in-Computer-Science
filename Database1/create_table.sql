//Name: Harikrishnan Gopal Janakiraman
//ID: 01372580


create table weather(Date1 date, City varchar2(25), Weather varchar2(10) not null, Constraint pk_weather primary key (Date1,City));



create table Teams(Team_Name Varchar2(40) Constraint pk_teams primary key, Coach varchar2(20), Conference varchar2(20) not null, Division varchar2(10));



create table stadium(Stadium_Name varchar2(40), State varchar2(25) not null, City varchar2(30) not null, Roof varchar2(20), Surface varchar2(15) not null,Constraint pk_stadium primary key (Stadium_Name));



Create table Primary_Team(Stadium_name varchar2(40), Primary_team varchar2(40), Primary key (stadium_name,primary_team), Constraint fk_sname foreign key (Stadium_Name) references Stadium(Stadium_name) ON DELETE CASCADE, Constraint fk_pteam foreign key (primary_team) references Teams(Team_name) ON DELETE CASCADE);



Create table Matches (Sections varchar2(20) not null, Day varchar2(10) not null, Date1 date not null, Venue varchar2(40), Winner varchar2(40), Loser varchar2(40), Points_of_winner Number not null, Points_of_loser Number not null, Yards_of_winner Number, Yards_of_loser Number, Attendance Number not null, Constraint fk_venue_sname foreign key (venue) references stadium (Stadium_Name) ON DELETE SET NULL, Constraint fk_winner_team foreign key(winner) references Teams(Team_Name) ON DELETE SET NULL, Constraint fk_loser_team foreign key(Loser) references Teams(Team_Name) ON DELETE SET NULL,Constraint pk_match Primary Key (Date1,Venue));