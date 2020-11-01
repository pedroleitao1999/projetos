package pt.tecnico.sauron.common;

public class Eye {

    private String nome;
    private Coords coords;

    public Eye(String nome, double latitude, double longitude) {
        this.nome = nome;
        this.coords = new Coords(latitude, longitude);
    }

    public String getNome(){
        return nome;
    }

    public void setNome(String novoNome){
        nome = novoNome;
    }

    public Coords getCoords(){
        return coords;
    }

}
