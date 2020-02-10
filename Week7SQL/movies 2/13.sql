

SELECT COUNT(title) FROM movies JOIN stars ON movies.id = stars.movie_id JOIN
people ON stars.person_id = people.id
WHERE people.id = (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958);