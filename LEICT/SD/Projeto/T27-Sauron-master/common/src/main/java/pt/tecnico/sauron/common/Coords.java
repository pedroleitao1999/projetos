package pt.tecnico.sauron.common;

public class Coords {

    double lat;
    double lon;

    public Coords(double lat, double lon){
        this.lat = lat;
        this.lon = lon;
    }

    public void setLat(double lat){
        this.lat = lat;
    }

    public double getLat() {
        return lat;
    }

    public void setLon(double lon) {
        this.lon = lon;
    }

    public double getLon() {
        return lon;
    }

}
