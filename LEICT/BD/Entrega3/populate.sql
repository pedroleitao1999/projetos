insert into local_publico values (75.282182, 48.929212, 'Surf');
insert into local_publico values (88.304291, 20.120932, 'Leaf Village');
insert into local_publico values (10.292182, 1.483294, 'Cascais Shopping');
insert into local_publico values (102.392910, -189.293219, 'Estadio Nacional');
insert into local_publico values (-39.293920, -2.392912, 'Bowlling');
insert into local_publico values (10.493923, 2.485412, 'Livraria');
insert into local_publico values (43.292012, -40.495384, 'Decatlon');
insert into local_publico values (70.483812, 3.120394, 'Parque dos Poetas');

insert into item values (1, 'Obstaculo', 'Oeiras', 75.282182, 48.929212);
insert into item values (2, 'Sinal', 'Cascais', 10.292182, 1.483294);
insert into item values (3, 'Rampa', 'Cascais', 10.292182, 1.483294);
insert into item values (4, 'Semaforo de transito', 'Beja', -39.293920, -2.392912);
insert into item values (5, 'Folhas', 'Leaf Village', 88.304291, 20.120932);
insert into item values (6, 'Parque de estacionamento', 'Cascais', 10.493923, 2.485412);
insert into item values (7, 'Escadas', 'Cascais', 10.292182, 1.483294);
insert into item values (8, 'Ondas', 'Oeiras', 75.282182, 48.929212);

insert into anomalia values (1, '((2,1),(5,7))', 'image1.jpg', 'POR', '2019-11-20 03:50:43', 'Nao da para surfar.', True);
insert into anomalia values (2, '((3,1),(7,13))', 'image2.jpg', 'ENG', '2019-03-14 11:54:03', 'Nao esta explicito.', False);
insert into anomalia values (3, '((12,4),(23,8))', 'image3.png', 'ENG', '2019-01-21 23:47:56', 'Nao mostra tudo.', True);
insert into anomalia values (4, '((5,7),(6,8))', 'foto.png', 'GER', '2019-07-26 11:21:23', 'Ve-se mal.', False);
insert into anomalia values (5, '((6,5),(8,12))', 'este.png', 'RUS', '2019-12-11 17:43:23', 'Perigoso.', False);
insert into anomalia values (6, '((14,9),(17,11))', 'outro.jpg', 'HUN', '2019-04-15 07:38:12', 'Mal sinalizado.', True);

insert into anomalia_traducao values (2, '((4,6),(7,19))', 'FR');
insert into anomalia_traducao values (3, '((19,7),(28,12))', 'RUS');
insert into anomalia_traducao values (4, '((6,13),(10,15))', 'ITA');
insert into anomalia_traducao values (5, '((11,7),(15,14))', 'POR');
insert into anomalia_traducao values (6, '((15,10),(16,11))', 'ESP');

insert into duplicado values (1, 3);

insert into utilizador values ('joao@gmail.com', 'palavra');
insert into utilizador values ('jorge@mail.pt', 'passe');
insert into utilizador values ('miguel@livemail.com.pt', 'minhapasse');
insert into utilizador values ('madalena@sapo.pt', 'password');

insert into utilizador_qualificado values ('joao@gmail.com');
insert into utilizador_qualificado values ('madalena@sapo.pt');

insert into utilizador_regular values ('jorge@mail.pt');
insert into utilizador_regular values ('miguel@livemail.com.pt');

insert into incidencia values (1, 8, 'joao@gmail.com');
insert into incidencia values (4, 4, 'miguel@livemail.com.pt');
insert into incidencia values (2, 4, 'joao@gmail.com');
insert into incidencia values (5, 4, 'miguel@livemail.com.pt');
insert into incidencia values (6, 6, 'jorge@mail.pt');
insert into incidencia values (3, 3, 'madalena@sapo.pt');

insert into proposta_de_correcao values ('joao@gmail.com', default, '2019-02-12 11:52:02', 'Tentar melhorar.');

insert into correcao values ('joao@gmail.com', 1, 1);
