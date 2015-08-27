//Name: Harikrishnan Gopal Janakiraman

//ID: 01372580

Q1: select distinct(coach),conference from matches k inner join teams t on k.loser=k.team_name where k.coach LIKE 'Mi%' and k.date1 between to_date('06-sep-2013') and to_date('16-sep-2013');


Q2: with table1 as (select winner,count(winner) as wins from matches k group by winner)
select k.team_name,t1.wins from teams k inner join table1 t1 on T.team_name = t1.winner order by t1.wins desc;

Q3: with table1 as(select * from (select date1,venue,attendance,s1.city from matches inner join stadium s1 on matches.venue=s1.Stadium_name order by attendance desc) where rownum<=10)
select table1.date1,table1.venue,table1.attendance,w1.weather from table1 inner join weather w1 on table1.date1=w1.date1 and table1.city = w1.city order by table1.attendance desc;​

Q4: with table1 as (select state,count(*) as no_of_stadiums from stadium group by state)
select state from table1 where no_of_stadiums > 1;

Q5: select distinct(coach) from teams where team_name in ((select winner from matches where venue = 'Gillette Stadium') union (select loser from matches where venue = 'Gillette Stadium'));

Q6: with table1 as (select m1.venue,m1.date1,m1.winner as team,s1.city from matches m1 inner JOIN stadium s1 on m1.venue = s1.stadium_name where s1.roof in ('Outdoors','Half') union select m1.venue,m1.date1,m1.loser as team,s1.city from matches m1 inner JOIN stadium s1 on m1.venue = s1.stadium_name where s1.roof in ('Outdoors','Half'))‫
select distinct(team) from table1 inner JOIN weather w1 on table1.date1 =w1.date1 and table1.city = w1.city where length(w1.weather) > 3;

Q7: with table1 as (select m1.winner as team,sum(m1.yards_of_winner) as totalyards, count(m1.winner) as noOfGames from matches m1 group by m1.winner 
union select m1.loser as team,sum(m1.yards_of_loser) as totalyards, count(m1.loser) as noOfGames from matches m1 group by m1.loser)
select distinct(t1.team), (t1.totalyards+t2.totalyards)/(t1.noOfGames+t2.noOfGames) as avgYards from table1 t1 inner join table1 t2 on t1.team=t2.team and t1.totalYards != t2.totalYards;

Q8: with table1 as (select m1.venue,m1.winner as team,s1.surface from matches m1 inner JOIN stadium s1 on m1.venue = s1.stadium_name union select m1.venue, m1.loser as team,s1.surface from matches m1 inner JOIN stadium s1 on m1.venue = s1.stadium_name)
select distinct(team) from table1 where team not in (select t1.team from table1 t1 where t1.surface ='A-turf');

Q9: delete  from weather where date1 not in  (select date1 from matches);

