select nome, latitude, longitude
from local_publico
natural join item
natural join incidencia
natural join anomalia
group by nome, latitude, longitude
having count (nome) = (
  select MAX(count)
  from (
    select nome, count(nome) as count
    from local_publico
    natural join item
    natural join incidencia
    natural join anomalia
    group by nome
  ) as foo
);

SELECT email FROM utilizador_regular
NATURAL JOIN incidencia
NATURAL JOIN anomalia
NATURAL JOIN anomalia_traducao
WHERE (ts <= '2019-06-30 23:59:59' AND ts >= '2019-01-01 00:00:00')
GROUP BY email
HAVING count (email) = (SELECT MAX(count) FROM (
  SELECT email, count(email) AS count
  FROM utilizador_regular
  NATURAL JOIN incidencia
  NATURAL JOIN anomalia
  NATURAL JOIN anomalia_traducao
  WHERE (ts <= '2019-06-30 23:59:59' AND ts >= '2019-01-01 00:00:00')
  GROUP BY email)
  AS foo
);

SELECT email FROM utilizador
NATURAL JOIN incidencia
NATURAL JOIN item
NATURAL JOIN anomalia
WHERE (latitude > 39.336775 AND ts >= '2019-01-01 00:00:00' AND ts <= '2019-12-31 23:59:59');

SELECT email FROM utilizador_qualificado
NATURAL JOIN incidencia
NATURAL JOIN item
NATURAL JOIN anomalia
WHERE latitude < '39.336775'
GROUP BY email
EXCEPT (
  SELECT email FROM utilizador_qualificado
  NATURAL JOIN proposta_de_correcao
  GROUP BY email
);
