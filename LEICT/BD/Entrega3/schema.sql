drop table local_publico cascade;
drop table item cascade;
drop table anomalia cascade;
drop table anomalia_traducao cascade;
drop table duplicado cascade;
drop table utilizador cascade;
drop table utilizador_qualificado cascade;
drop table utilizador_regular cascade;
drop table incidencia cascade;
drop table proposta_de_correcao cascade;
drop table correcao cascade;

create table local_publico(
  latitude numeric(9,6) not null,
  longitude numeric(9,6) not null,
  nome varchar(30) not null,
  constraint pk_local_publico primary key(latitude, longitude)
);

create table item(
  item_id integer not null,
  descricao_item text not null,
  localizacao varchar(30) not null,
  latitude numeric(9,6) not null,
  longitude numeric(9,6) not null,
  constraint pk_item primary key(item_id),
  constraint fk_item foreign key(latitude, longitude) references local_publico(latitude, longitude)
);

create table anomalia(
  anomalia_id integer not null,
  zona varchar(30) not null,
  imagem varchar(2083) not null,
  lingua char(3) not null,
  ts timestamp not null,
  descricao_anomalia text not null,
  tem_anomalia_redacao boolean not null,
  constraint pk_anomalia primary key(anomalia_id)
);

create table anomalia_traducao(
  anomalia_id integer not null,
  zona2 varchar(30) not null,
  lingua2 char(3) not null,
  constraint pk_anomalia_traducao primary key(anomalia_id),
  constraint fk_anomalia_traducao foreign key(anomalia_id) references anomalia(anomalia_id)
);

create table duplicado(
  item1 integer not null,
  item2 integer not null,
  CHECK (item1<item2),
  constraint pk_duplicado primary key(item1, item2),
  constraint fk_duplicado_item1 foreign key(item1) references item(item_id),
  constraint fk_duplicado_item2 foreign key(item2) references item(item_id)
);

create table utilizador(
  email varchar(254) not null,
  password varchar(30) not null,
  constraint pk_utilizador primary key(email)
);

create table utilizador_qualificado(
  email varchar(254) not null,
  constraint pk_utilizador_qualificado primary key(email),
  constraint fk_utilizador_qualificado foreign key(email) references utilizador(email)
);

create table utilizador_regular(
  email varchar(254) not null,
  constraint pk_utilizador_regular primary key(email),
  constraint fk_utilizador_regular foreign key(email) references utilizador(email)
);

create table incidencia(
  anomalia_id integer not null,
  item_id integer not null,
  email varchar(254) not null,
  constraint pk_incidencia primary key(anomalia_id),
  constraint fk_incidencia_anomalia foreign key(anomalia_id) references anomalia(anomalia_id),
  constraint fk_incidencia_item foreign key(item_id) references item(item_id),
  constraint fk_incidencia_email foreign key(email) references utilizador(email)
);

create table proposta_de_correcao(
  email varchar(254) not null,
  nro serial not null,
  data_hora timestamp not null,
  texto text not null,
  primary key(email, nro),
  foreign key(email) references utilizador_qualificado(email)
);

create table correcao(
  email varchar(254),
  nro integer not null,
  anomalia_id integer not null,
  primary key(email, nro, anomalia_id),
  foreign key(email, nro) references proposta_de_correcao(email, nro),
  foreign key(anomalia_id) references incidencia(anomalia_id)
);
