import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class EventPage extends StatefulWidget {
  @override
  _EventPageState createState() => _EventPageState();
}

class _EventPageState extends State<EventPage> {
  @override
  Widget build(BuildContext context) {
    return Container(
      child: ListView(
        children: <Widget>[
          SizedBox(height: 10.0,),
          Container(
            //width: MediaQuery.of(context).size.width -30.0,
            //height: MediaQuery.of(context).size.height - 50.0,
            child: new Column(
              children: <Widget>[
                _buildCard(
                    1,
                    'Listen (2020)',
                    'M14\nGenre: Drama \nDirector: Ana Rocha\n In the suburbs of London, Bela and Jota face serious challenges when Social Services raise doubts regarding the safety of their children. ',
                    'assets/listen.png',
                    'Cinema',
                    false,
                    context),
                _buildCard(
                    2,
                    'NOS Alive',
                    'A music and arts festival which takes place in the Algés riverside in Oerias. Guests include Kendrick Lamar, Taylor Swift, Billie Eilish. Khalid, Alt-J and the Lumineers.',
                    'assets/nos_alive.png',
                    'Concert',
                    false,
                    context),
                _buildCard(
                    3,
                    'Pi 100 Pé',
                    'M06\nComedy show by Fernando Rocha.\nSuper Bock Arena\n Campo Pequeno\nTickets available everywhere',
                    'assets/pi100pe.png',
                    'Comedy',
                    true,
                    context),
              ],
            )
            ),
        ],
      ),
    );
  }
}

Widget _buildCard(int id, String name, String description, String imagePath, String type, bool isFavorite, context){
  return Padding(
    padding: EdgeInsets.only(left: 1.0, top: 1.0, right: 1.0, bottom: 8.0),
    child: Container(
            decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(20.0),
                boxShadow: [
                  BoxShadow(
                      color: Colors.grey.withOpacity(0.2),
                      spreadRadius: 3.0,
                      blurRadius: 5.0)
                ],
                color: Colors.white),

          child: Row(
            children: <Widget>[
              Expanded(
                child: Container(
                  //fit: BoxFit.contain,
                  alignment: Alignment.centerLeft,
                  child: Image.asset(imagePath),
                )),
              Expanded(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Padding(
                      padding: EdgeInsets.only(left: 8.0),
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        //crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Container(
                              decoration: BoxDecoration(
                              shape: BoxShape.rectangle,
                              borderRadius: BorderRadiusDirectional.all(Radius.circular(20.0)),
                              color: Colors.orange
                            ),
                            child: Padding(
                              padding: EdgeInsets.all(6.0),
                              child: Text(type, style: TextStyle(
                                  fontFamily: 'Roboto',
                                  fontSize: 10.0,
                                  fontWeight: FontWeight.bold,
                                  color: Colors.white) ),
                            )
                          ),
                          isFavorite
                            ? IconButton(icon: Icon(Icons.favorite, color: Colors.deepOrange), onPressed: () => {isFavorite = !isFavorite})
                            : IconButton(icon: Icon(Icons.favorite_border, color: Colors.black), onPressed: () => {isFavorite = !isFavorite})
                        ],
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.only(left: 8.0, bottom: 8.0),
                      child: Text(name, style: TextStyle(
                          fontFamily: 'Roboto',
                          fontSize: 18.0,
                          fontWeight: FontWeight.bold,
                          color: Colors.deepOrange) ),
                    ),
                    Padding(
                      padding: EdgeInsets.all(8.0),
                      child: Text(description, style: TextStyle(
                          fontFamily: 'Roboto',
                          fontSize: 10.0,
                          fontWeight: FontWeight.normal,
                          color: Colors.black) ),
                    )

                  ],
                ),
              )
            ],
          )
    )
  );
}

