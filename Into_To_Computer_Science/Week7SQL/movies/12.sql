SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id JOIN
people ON stars.person_id = people.id
WHERE name IN ("Helena Bonham Carter", "Johnny Depp")
GROUP BY title
HAVING COUNT(title) > 1;