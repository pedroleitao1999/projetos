// Copyright 2018 The Flutter team. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';
import 'package:intl/intl.dart';
import 'dart:async';
import './firstPage.dart';
import './mainPayment.dart';

final _favorites = Set<int>();
final List<String> _myMessages = [];
bool BeatrizFriend = false;
bool RodrigoFriend = false;
bool hasGroupChat = false;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Culturally',
      theme: ThemeData(
        primaryColor: Colors.orange,
      ),
      home: FirstPage(),
    );
  }
}

class HomePage extends StatefulWidget {
  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> with SingleTickerProviderStateMixin {
  DateTime selectedDate = DateTime.now();
  OverlayEntry _overlayEntry;
  OverlayState overlayState;
  String dropdownValue = ' Alphabetical Asc.        ';
  double _currentPriceSliderValue = 100;
  double _currentCapacitySliderValue = 10;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      /* -------------------------------- App bar, at the top of the screen --------------------------*/
      appBar: AppBar(
        backgroundColor: Colors.orange,
        elevation: 9.0,
        leading: IconButton(
          icon: Icon(Icons.arrow_back, color: Colors.white),
          onPressed: () {
            Navigator.pop(context);
          },
        ),
        title: Text('Events List',
            style: TextStyle(
                fontFamily: 'Roboto',
                fontSize: 20.0,
                color: Colors.white)),
        actions: <Widget>[
          IconButton(
              icon: Icon(Icons.search, color: Colors.white),
              onPressed: () {}
          ),
          IconButton(
              icon: Icon(Icons.tune, color: Colors.white),
              onPressed: () {
                showFilterOverlay(context);
              }
          ),
        ],
      ),
/* ------------------------------- Main screen body, has event cards ------------------------------*/
      body: ListView(
        padding: EdgeInsets.all(10.0),
        children: <Widget>[
/* --------------------------------- Date picker box --------------------------------*/
          InkWell(
            onTap: () => _selectDate(context),
            child:
            Container(
              decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(20.0),
                  boxShadow: [
                    BoxShadow(
                        color: Colors.grey.withOpacity(0.2),
                        spreadRadius: 3.0,
                        blurRadius: 5.0)
                  ],
                  color: Colors.white),
              child: Column(
                children: [
                  Row(
                    children: [
                      Padding(
                        padding: EdgeInsets.only(left: 15, top: 15, bottom: 8),
                        child: Text('When', style: TextStyle(
                            fontFamily: 'Roboto',
                            fontSize: 16.0,
                            fontWeight: FontWeight.normal,
                            color: Colors.deepOrange)
                        ),
                      )
                    ],
                  ),
                  Padding(
                    padding: EdgeInsets.only(
                        left: 15.0, bottom: 15.0, right: 15.0),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Text("${DateFormat('EEEE, d MMM, yyyy').format(
                            selectedDate)}", style: TextStyle(
                            fontFamily: 'Roboto',
                            fontSize: 18.0,
                            fontWeight: FontWeight.normal,
                            color: Colors.black)
                        ),
                        Icon(Icons.calendar_today, color: Colors.black, size: 20,)
                      ],
                    ),
                  ),
                ],
              ),
            ),
          ),
          SizedBox(height: 15.0),

/* --------------------------------------- Event cards list ----------------------------------*/
          if(selectedDate.day.isOdd && _currentPriceSliderValue >= 8)
            _buildCard(
                1,
                'Listen (2020)',
                'M14\nGenre: Drama \nDirector: Ana Rocha\n In the suburbs of London, Bela and Jota face serious challenges when Social Services raise doubts regarding the safety of their children. ',
                'assets/listen.png',
                'Cinema',
                context),
          if(selectedDate.day.isEven && _currentPriceSliderValue >= 68)
            _buildCard(
                2,
                'NOS Alive',
                'A music and arts festival which takes place in the Algés riverside in Oerias.\n\n Guests include Kendrick Lamar, Taylor Swift, Billie Eilish. Khalid, Alt-J and the Lumineers.\n\nCome have the time of your life!',
                'assets/nos_alive.png',
                'Concert',
                context),
          if(selectedDate.day.isOdd && _currentPriceSliderValue >= 22)
            _buildCard(
                3,
                'Pi 100 Pé',
                'M06\n\nComedy show by Fernando Rocha.\nSit back, relax and have a laugh!\n\nSuper Bock Arena\n Campo Pequeno\nTickets available everywhere',
                'assets/pi100pe.png',
                'Comedy',
                context),
          if(selectedDate.day.isEven && _currentPriceSliderValue >= 18 )
            _buildCard(
                4,
                'Primeiro Modernismo às Novas Vanguardas do Século XX',
                'The Berardo Collection brings together a remarkable of works that marked the history of art of the twentieth century',
                'assets/berardo.png',
                'Museum',
                context),
          if(selectedDate.day.isOdd && _currentPriceSliderValue >= 18 )
            _buildCard(
                5,
                'Uma Mulher Não Chora',
                'M06\nMusical\nFor 80 minutes, the singers join the musical universe of Renato Júnior. Paying homage to the reality of being a woman in the twenty first century, lending their voice to eradicate violence against women.',
                'assets/teatro.png',
                'Theater',
                context),
        ],
      ),

      /* ------------------- Central, flaoting action button for the match -------------------*/

      floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
        backgroundColor: Colors.deepOrange,
        child: Image.asset('assets/groups.png', height: 35, width: 35,),
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,

      /* ---------------------- Bottom Navigation Bar with the 4 icons ----------------------*/

      bottomNavigationBar: _bottomAppBar(),

    );
  }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------- FUNCTION FOR CREATING  BOTTOM NAVIGATION  ---------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  Widget _bottomAppBar(){
    return BottomAppBar(
      shape: CircularNotchedRectangle(),
      notchMargin: 6.0,
      color: Colors.transparent,
      elevation: 9.0,
      clipBehavior: Clip.antiAlias,
      child: Container(
          height: 50.0,
          decoration: BoxDecoration(
              borderRadius: BorderRadius.only(
                  topLeft: Radius.circular(25.0),
                  topRight: Radius.circular(25.0)
              ),
              color: Colors.orange
          ),
          child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Container(
                    height: 50.0,
                    width: MediaQuery.of(context).size.width /2 - 30.0,
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        IconButton(
                            icon: Icon(Icons.home, color: Colors.white),
                            onPressed: pushHomepage
                        ),
                        IconButton(
                            icon: Icon(Icons.favorite, color: Colors.white),
                            onPressed: pushFavorites
                        ),
                      ],
                    )
                ),
                Container(
                    height: 50.0,
                    width: MediaQuery.of(context).size.width /2 - 30.0,
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        IconButton(
                            icon: Icon(Icons.forum, color: Colors.white),
                            onPressed: pushGroupChats),
                        IconButton(
                            icon: Icon(Icons.account_box, color: Colors.white),
                            onPressed: () { pushProfile(1, 'Joaquim Silva', 'January 30th 1998',
                                'joaquim@gmail.com', 'instagram.com/joaquims', '919000000',
                            'University student.  I like to watch movies, go to concerts and soccer. Looking for company to make new friends, hopefully!',
                                'assets/joaquim.png'); } ),
                      ],
                    )
                ),
              ]
          )
      ),
    );
  }

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------ CREATE EVENT CARDS GIVEN THE RELEVANT INFORMATION ------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  Widget _buildCard(int id, String name, String description, String imagePath, String type, context) {
    final isFavorite = _favorites.contains(id);
    return InkWell(
        onTap: () {pushEventDetails(id);},
        child: Padding(
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
                    /*------------------- Image on the left -------------------*/
                    Expanded(
                        child: Container(
                          //fit: BoxFit.contain,
                          alignment: Alignment.centerLeft,
                          child: Image.asset(imagePath),
                        )),
                    /*------------------- Info on the right -------------------*/
                    Expanded(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.only(left: 8.0,),
                            child: Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              //crossAxisAlignment: CrossAxisAlignment.start,
                              children: [
                                Container(
                                    decoration: BoxDecoration(
                                        shape: BoxShape.rectangle,
                                        borderRadius: BorderRadiusDirectional.all(
                                            Radius.circular(20.0)),
                                        color: Colors.orange
                                    ),
                                    child: Padding(
                                      padding: EdgeInsets.all(6.0),
                                      /* ------------------- Type orange tag -------------------*/
                                      child: Text(type, style: TextStyle(
                                          fontFamily: 'Roboto',
                                          fontSize: 10.0,
                                          fontWeight: FontWeight.bold,
                                          color: Colors.white)),
                                    )
                                ),
                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],
                            ),
                          ),
                          Padding(
                            padding: EdgeInsets.only(
                                left: 8.0, top: 8.0, bottom: 8.0, right: 8.0),
                            /* ------------------- Event Title -------------------*/
                            child: Text(name, style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 18.0,
                                fontWeight: FontWeight.bold,
                                color: Colors.deepOrange)),
                          ),
                          Padding(
                            padding: EdgeInsets.all(8.0),
                            /*------------------- Event description -------------------*/
                            child: Text(description, style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 10.0,
                                fontWeight: FontWeight.normal,
                                color: Colors.black)),
                          )

                        ],
                      ),
                    )
                  ],
                )
            )
        )
    );
  }

/*---------------------------------- FUNCTION FOR NAVIGATING TO HOME WITH BOTTOM BAR ICON ----------------------------------*/

  void pushHomepage(){
    Navigator.of(context).push(
      MaterialPageRoute<void>(
          builder: (BuildContext context) {
            return HomePage();
          }
      ),
    );
  }

  void pushMainPayment(){
    Navigator.of(context).push(
      MaterialPageRoute<void>(
          builder: (BuildContext context) {
            return mainPayment();
          }
      ),
    );
  }


  void cinemaOptions() {
    Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return Scaffold(
                  appBar: AppBar(
                      backgroundColor: Colors.orange,
                      elevation: 9.0,
                      leading: IconButton(
                        icon: Icon(Icons.arrow_back, color: Colors.white),
                        onPressed: () {
                          Navigator.pop(context);
                        },
                      ),
                      title: Text('Options',
                          style: TextStyle(
                              fontFamily: 'Roboto',
                              fontSize: 20.0,
                              color: Colors.white))),
                  floatingActionButton: FloatingActionButton(onPressed: () {},
                    backgroundColor: Colors.deepOrange,
                    child: Image.asset('assets/groups.png', height: 35, width: 35,),
                  ),
                  floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,
                  bottomNavigationBar: BottomAppBar(
                      shape: CircularNotchedRectangle(),
                      notchMargin: 6.0,
                      color: Colors.transparent,
                      elevation: 9.0,
                      clipBehavior: Clip.antiAlias,
                      child: Container(
                          height: 50.0,
                          decoration: BoxDecoration(
                              borderRadius: BorderRadius.only(
                                  topLeft: Radius.circular(25.0),
                                  topRight: Radius.circular(25.0)
                              ),
                              color: Colors.orange
                          ),
                          child: Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Container(
                                    height: 50.0,
                                    width: MediaQuery.of(context).size.width /2 - 30.0,
                                    child: Row(
                                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                                      children: <Widget>[
                                        IconButton(
                                            icon: Icon(Icons.home, color: Colors.white),
                                            onPressed: null),
                                        IconButton(
                                          icon: Icon(Icons.favorite, color: Colors.white),
                                          //onPressed: pushFavorites
                                        ),
                                      ],
                                    )
                                ),
                                Container(
                                    height: 50.0,
                                    width: MediaQuery.of(context).size.width /2 - 30.0,
                                    child: Row(
                                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                                      children: <Widget>[
                                        IconButton(
                                            icon: Icon(Icons.forum, color: Colors.white),
                                            onPressed: null),
                                        IconButton(
                                            icon: Icon(Icons.account_box, color: Colors.white),
                                            onPressed: null),
                                      ],
                                    )
                                ),
                              ]
                          )
                      )),
                  body: Stack(
                      children: <Widget>[
                        new Positioned.fill(
                            top: -500,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                    color: Colors.deepOrange,
                                    textColor: Colors.white,
                                    padding: EdgeInsets.all(8.0),
                                    minWidth: 350,
                                    shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                    onPressed: () {
                                      Navigator.push(
                                        context,
                                        MaterialPageRoute(builder: (context) => mainPayment()),
                                      );
                                    },
                                    child: Text(
                                      "18:00",
                                      style: TextStyle(fontSize: 20.0),
                                    )))),
                        new Positioned.fill(
                            top: -350,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                  color: Colors.deepOrange,
                                  textColor: Colors.white,
                                  padding: EdgeInsets.all(8.0),
                                  minWidth: 350,
                                  shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                  onPressed: () {
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(builder: (context) => mainPayment()),
                                    );
                                  },
                                  child: Text(
                                    "20:00",
                                    style: TextStyle(fontSize: 20.0),
                                  ),
                                ))),
                        new Positioned.fill(
                            top: -200,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                  color: Colors.deepOrange,
                                  textColor: Colors.white,
                                  padding: EdgeInsets.all(8.0),
                                  minWidth: 350,
                                  shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                  onPressed: () {
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(builder: (context) => mainPayment()),
                                    );
                                  },
                                  child: Text(
                                    "22:00",
                                    style: TextStyle(fontSize: 20.0),
                                  ),
                                ))),
                        new Positioned.fill(
                          top: -50,
                          child: Align(
                              alignment: Alignment.center,
                              child: FlatButton(
                                  color: Colors.deepOrange,
                                  textColor: Colors.white,
                                  padding: EdgeInsets.all(8.0),
                                  minWidth: 350,
                                  shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                  onPressed: () {
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(builder: (context) => mainPayment()),
                                    );
                                  },
                                  child: Text(
                                    "00:00",
                                    style: TextStyle(fontSize: 20.0),
                                  ))),
                        )]));}));
  }

  void NosAliveOptions(){
    Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return Scaffold(
                  appBar: AppBar(
                      backgroundColor: Colors.orange,
                      elevation: 9.0,
                      leading: IconButton(
                        icon: Icon(Icons.arrow_back, color: Colors.white),
                        onPressed: () {
                          Navigator.pop(context);
                        },
                      ),
                      title: Text('Options',
                          style: TextStyle(
                              fontFamily: 'Roboto',
                              fontSize: 20.0,
                              color: Colors.white))),
                  floatingActionButton: FloatingActionButton(onPressed: () {},
                    backgroundColor: Colors.deepOrange,
                    child: Image.asset('assets/groups.png', height: 35, width: 35,),
                  ),
                  floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,
                  bottomNavigationBar: BottomAppBar(
                      shape: CircularNotchedRectangle(),
                      notchMargin: 6.0,
                      color: Colors.transparent,
                      elevation: 9.0,
                      clipBehavior: Clip.antiAlias,
                      child: Container(
                          height: 50.0,
                          decoration: BoxDecoration(
                              borderRadius: BorderRadius.only(
                                  topLeft: Radius.circular(25.0),
                                  topRight: Radius.circular(25.0)
                              ),
                              color: Colors.orange
                          ),
                          child: Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Container(
                                    height: 50.0,
                                    width: MediaQuery.of(context).size.width /2 - 30.0,
                                    child: Row(
                                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                                      children: <Widget>[
                                        IconButton(
                                            icon: Icon(Icons.home, color: Colors.white),
                                            onPressed: null),
                                        IconButton(
                                          icon: Icon(Icons.favorite, color: Colors.white),
                                          //onPressed: pushFavorites
                                        ),
                                      ],
                                    )
                                ),
                                Container(
                                    height: 50.0,
                                    width: MediaQuery.of(context).size.width /2 - 30.0,
                                    child: Row(
                                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                                      children: <Widget>[
                                        IconButton(
                                            icon: Icon(Icons.forum, color: Colors.white),
                                            onPressed: null),
                                        IconButton(
                                            icon: Icon(Icons.account_box, color: Colors.white),
                                            onPressed: null),
                                      ],
                                    )
                                ),
                              ]
                          )
                      )),
                  body: Stack(
                      children: <Widget>[
                        new Positioned.fill(
                            top: -500,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                    color: Colors.deepOrange,
                                    textColor: Colors.white,
                                    padding: EdgeInsets.all(8.0),
                                    minWidth: 350,
                                    shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                    onPressed: () {
                                      Navigator.push(
                                        context,
                                        MaterialPageRoute(builder: (context) => mainPayment()),
                                      );
                                    },
                                    child: Text(
                                      "Daily ticket",
                                      style: TextStyle(fontSize: 20.0),
                                    )))),
                        new Positioned.fill(
                            top: -350,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                  color: Colors.deepOrange,
                                  textColor: Colors.white,
                                  padding: EdgeInsets.all(8.0),
                                  minWidth: 350,
                                  shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                  onPressed: () {
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(builder: (context) => mainPayment()),
                                    );
                                  },
                                  child: Text(
                                    "3-day Pass",
                                    style: TextStyle(fontSize: 20.0),
                                  ),
                                ))),
                        new Positioned.fill(
                            top: -200,
                            child: Align(
                                alignment: Alignment.center,
                                child: FlatButton(
                                  color: Colors.deepOrange,
                                  textColor: Colors.white,
                                  padding: EdgeInsets.all(8.0),
                                  minWidth: 350,
                                  shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                                  onPressed: () {
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(builder: (context) => mainPayment()),
                                    );
                                  },
                                  child: Text(
                                    "4-day Pass",
                                    style: TextStyle(fontSize: 20.0),
                                  ),
                                )))
                      ]));}));
  }

/*---------------------------------- FUNCTION FOR NAVIGATING TO REVIEWS WITH BOTTOM BAR ICON ----------------------------------*/
  void pushReviews(){
    //VAI E CONSTROI ECRA REVIEWS
    Navigator.of(context).push(
      MaterialPageRoute<void>(
          builder: (BuildContext context) {

            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Reviews',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.only(top: 30.0),
                  children: <Widget>[
                    Expanded(
                        child: Container(
                          //fit: BoxFit.contain,
                          alignment: Alignment.centerLeft,
                          child: Image.asset('assets/reviews.PNG'),
                        )
                    ),
                    /*
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Text("No reviews yet", style: TextStyle(fontSize: 30, color: Colors.grey, fontFamily: 'Roboto'),),
                      ],
                    ),
                     */
                  ]),
              floatingActionButton: FloatingActionButton(onPressed: () {},
                backgroundColor: Colors.deepOrange,
                child: Icon(Icons.rate_review, size: 25,),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,
            );


          }
      ),
    );
  }

/*gohere*/

/*---------------------------------- FUNCTION FOR NAVIGATING TO EVENT DETAILS  ----------------------------------*/
  void pushEventDetails(int id){
    bool isFavorite = _favorites.contains(id);
// CASO PARA CADA EVENTO, DESENHA  O ECRA
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {
          if (id == 1){
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.all(10.0),
                children: <Widget>[

                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/listen_banner.png'),
                      )
                  ),
                  Expanded(
                    child:
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.all(20.0),
                            child:
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Text("Listen (2020)",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 20, color: Colors.deepOrange),),

                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],),
                          ),

                          Wrap(
                            direction: Axis.horizontal,
                            children: [
                              Padding(padding: EdgeInsets.only(left: 20, right: 15), child:
                              Text("M14. In the suburbs of London, Bela and Jota face serious challenges when Social Services raise doubts regarding the safety of their children. ",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black))),
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.place, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Rua Pêro da Covilhã, 36 1400-296 Lisboa",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.access_time, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("December 7, 18:00, 20:00, 22:00, 24:00",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.call, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("cinemas.nos.pt",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.shopping_cart, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("8€",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushReviews,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 40, right: 40),
                                            child: Text('   See reviews   ', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: cinemaOptions,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 35, right: 35),
                                            child: Text('Purchase a ticket', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),

                        ]
                    ),
                  )
                ],
              ),
              floatingActionButton: FloatingActionButton(onPressed: pushMatchMadeScreen,
                backgroundColor: Colors.deepOrange,
                child: Image.asset('assets/groups.png', height: 35, width: 35,),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,


            );
          }
          if (id == 2){
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.all(10.0),
                children: <Widget>[

                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/nos_alive_banner.png'),
                      )
                  ),
                  Expanded(
                    child:
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.all(20.0),
                            child:
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Text("NOS Alive",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 20, color: Colors.deepOrange),),

                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],),
                          ),

                          Wrap(
                            direction: Axis.horizontal,
                            children: [
                              Padding(padding: EdgeInsets.only(left: 20, right: 15), child:
                              Text("A music and arts festival which takes place in the Algés riverside in Oerias.  Guests include Kendrick Lamar, Taylor Swift, Billie Eilish. Khalid, Alt-J and the Lumineers",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black))),
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.place, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Rua Pêro da Covilhã, 36 1400-296 Lisboa",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.access_time, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("December 8, 17:00 - 24:00",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.call, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("213 933 770",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.shopping_cart, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Diary 68€, 3-Day Pass 159€, 4-Day Pass 189€",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushReviews,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 40, right: 40),
                                            child: Text('   See reviews   ', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: NosAliveOptions,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 35, right: 35),
                                            child: Text('Purchase a ticket', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),

                        ]
                    ),
                  )
                ],
              ),
              floatingActionButton: FloatingActionButton(onPressed: pushMatchMadeScreen,
                backgroundColor: Colors.deepOrange,
                child: Image.asset('assets/groups.png', height: 35, width: 35,),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,


            );
          }
          if (id == 3){
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.all(10.0),
                children: <Widget>[

                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/pi100pe_banner.png'),
                      )
                  ),
                  Expanded(
                    child:
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.all(20.0),
                            child:
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Text("Pi 100 Pé",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 20, color: Colors.deepOrange),),

                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],),
                          ),

                          Wrap(
                            direction: Axis.horizontal,
                            children: [
                              Padding(padding: EdgeInsets.only(left: 20, right: 15), child:
                              Text("M06. Comedy show by Fernando Rocha. Super Bock Arena Campo Pequeno. Tickets available everywhere",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black))),
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.place, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Pavilhão Rosa Mota",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.access_time, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("December 7, 19, 22:00",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.call, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("facebook.com/100peoficial",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.shopping_cart, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("22€",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushReviews,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 40, right: 40),
                                            child: Text('   See reviews   ', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushMainPayment,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 35, right: 35),
                                            child: Text('Purchase a ticket', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),

                        ]
                    ),
                  )
                ],
              ),
              floatingActionButton: FloatingActionButton(onPressed: pushMatchMadeScreen,
                backgroundColor: Colors.deepOrange,
                child: Image.asset('assets/groups.png', height: 35, width: 35,),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,


            );
          }
          if (id == 4){
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.all(10.0),
                children: <Widget>[

                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/berardo_banner.png'),
                      )
                  ),
                  Expanded(
                    child:
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.all(20.0),
                            child:
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Text("Primeiro Modernismo às Novas\nVanguardas do Século XX",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 20, color: Colors.deepOrange),),

                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],),
                          ),

                          Wrap(
                            direction: Axis.horizontal,
                            children: [
                              Padding(padding: EdgeInsets.only(left: 20, right: 15), child:
                              Text("The Berardo Collection brings together a remarkable of works that marked the history of art of the twentieth century",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black))),
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.place, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Praça do Império 1449-003 Lisboa",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.access_time, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Everyday, 10:00 - 19:00",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.call, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("21 361 287 8",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.shopping_cart, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("18€",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushReviews,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 40, right: 40),
                                            child: Text('   See reviews   ', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushMainPayment,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 35, right: 35),
                                            child: Text('Purchase a ticket', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),

                        ]
                    ),
                  )
                ],
              ),
              floatingActionButton: FloatingActionButton(onPressed: pushMatchMadeScreen,
                backgroundColor: Colors.deepOrange,
                child: Image.asset('assets/groups.png', height: 35, width: 35,),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,


            );
          }
          if (id == 5){
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),

              body: ListView(
                //padding: EdgeInsets.all(10.0),
                children: <Widget>[

                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/teatro_banner.png'),
                      )
                  ),
                  Expanded(
                    child:
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.all(20.0),
                            child:
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Text("Uma Mulher Não Chora",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 20, color: Colors.deepOrange),),

                                IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    isFavorite ? Icons.favorite : Icons.favorite_border,
                                    color: isFavorite ? Colors.red : null,
                                  ),
                                  onPressed: (){
                                    setState(() {
                                      if(isFavorite){
                                        _favorites.remove(id);
                                      } else {
                                        _favorites.add(id);
                                      }
                                    });
                                  },
                                )
                              ],),
                          ),

                          Wrap(
                            direction: Axis.horizontal,
                            children: [
                              Padding(padding: EdgeInsets.only(left: 20, right: 15), child:
                              Text("For 80 minutes, the singers join the musical universe of Renato Júnior. Paying homage to the reality of being a woman in the twenty first century, lending their voice to eradicate violence against women.",
                                  style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black))),
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.place, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("Av. Frei Miguel Contreiras 52-53, Lisboa",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.access_time, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("December 7, 21:00",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.call, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("teatromariamatos.pt",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                            children: <Widget> [
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child: Icon(Icons.shopping_cart, color: Colors.black),
                              ),
                              Padding(
                                padding: EdgeInsets.only(left: 15, top: 15),
                                child:
                                Text("18€",
                                    style: TextStyle(fontFamily: 'Roboto', fontSize: 14, color: Colors.black)),
                              )
                            ],
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushReviews,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 40, right: 40),
                                            child: Text('   See reviews   ', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children:[
                                Padding(
                                    padding: EdgeInsets.only(top:15, left: 15),
                                    child:
                                    InkWell(
                                      onTap: pushMainPayment,
                                      child:
                                      Container(
                                          decoration: BoxDecoration(
                                            shape: BoxShape.rectangle,
                                            borderRadius: BorderRadiusDirectional.all(
                                                Radius.circular(25.0)),
                                            color: Colors.orange,
                                          ),
                                          child: Padding(
                                            padding: EdgeInsets.only(top: 10, bottom: 10, left: 35, right: 35),
                                            child: Text('Purchase a ticket', style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 18.0,
                                                fontWeight: FontWeight.bold,
                                                color: Colors.white,
                                                decoration: TextDecoration.none)),
                                          )
                                      ),
                                    )
                                )
                              ]
                          ),

                        ]
                    ),
                  )
                ],
              ),
              floatingActionButton: FloatingActionButton(onPressed: pushMatchMadeScreen,
                backgroundColor: Colors.deepOrange,
                child: Image.asset('assets/groups.png', height: 35, width: 35),
              ),
              floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,


            );
          }else {
            return Scaffold(
              appBar: AppBar(
                backgroundColor: Colors.orange,
                elevation: 9.0,
                leading: IconButton(
                  icon: Icon(Icons.arrow_back, color: Colors.white),
                  onPressed: () {
                    Navigator.pop(context);
                  },
                ),
                title: Text('Event details',
                    style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 20.0,
                        color: Colors.white)),
              ),
            );
          }
        }, // ...to here.
      ),
    );
  }


/*---------------------------------- FUNCTION FOR NAVIGATING TO FAVORITES WITH BOTTOM BAR ICON ----------------------------------*/
  void pushFavorites() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {


          return Scaffold(
            appBar: AppBar(
              backgroundColor: Colors.orange,
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Favorites',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(
                padding: EdgeInsets.all(10.0),
                children: [
                  /*Hard coded favorites check*/
                  if(_favorites.contains(1))
                    _buildCard(
                        1,
                        'Listen (2020)',
                        'M14\nGenre: Drama \nDirector: Ana Rocha\n In the suburbs of London, Bela and Jota face serious challenges when Social Services raise doubts regarding the safety of their children. ',
                        'assets/listen.png',
                        'Cinema',
                        context),
                  if(_favorites.contains(2))
                    _buildCard(
                        2,
                        'NOS Alive',
                        'A music and arts festival which takes place in the Algés riverside in Oerias.\n\n Guests include Kendrick Lamar, Taylor Swift, Billie Eilish. Khalid, Alt-J and the Lumineers.\n\nCome have the time of your life!',
                        'assets/nos_alive.png',
                        'Concert',
                        context),
                  if(_favorites.contains(3))
                    _buildCard(
                        3,
                        'Pi 100 Pé',
                        'M06\n\nComedy show by Fernando Rocha.\nSit back, relax and have a laugh!\n\nSuper Bock Arena\n Campo Pequeno\nTickets available everywhere',
                        'assets/pi100pe.png',
                        'Comedy',
                        context),
                  if(_favorites.contains(4))
                    _buildCard(
                        4,
                        'Primeiro Modernismo às Novas Vanguardas do Século XX',
                        'The Berardo Collection brings together a remarkable of works that marked the history of art of the twentieth century',
                        'assets/berardo.png',
                        'Museum',
                        context),
                  if(_favorites.contains(5))
                    _buildCard(
                        5,
                        'Uma Mulher Não Chora',
                        'M06\nMusical\nFor 80 minutes, the singers join the musical universe of Renato Júnior. Paying homage to the reality of being a woman in the twenty first century, lending their voice to eradicate violence against women.',
                        'assets/teatro.png',
                        'Theater',
                        context),
                ]
            ),
            floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
              backgroundColor: Colors.deepOrange,
              child: Image.asset('assets/groups.png', height: 35, width: 35,),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,

            bottomNavigationBar: _bottomAppBar(),

          );
        }, // ...to here.
      ),
    );
  }


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------- DATE PICKER THINGS --------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/


  /// This decides which day will be enabled
  /// This will be called every time while displaying day in calender.
  bool _decideWhichDayToEnable(DateTime day) {
    if ((day.isAfter(DateTime.now().subtract(Duration(days: 1))) &&
        day.isBefore(DateTime.now().add(Duration(days: 1))))) {
      return true;
    }
    return false;
  }

  _selectDate(BuildContext context) {
    final ThemeData theme = Theme.of(context);
    assert(theme.platform != null);
    switch (theme.platform) {
      case TargetPlatform.android:
      case TargetPlatform.fuchsia:
      case TargetPlatform.linux:
      case TargetPlatform.windows:
        return buildMaterialDatePicker(context);
      case TargetPlatform.iOS:
      case TargetPlatform.macOS:
        return buildCupertinoDatePicker(context);
    }
  }

  buildCupertinoDatePicker(BuildContext context) {
    showModalBottomSheet(
        context: context,
        builder: (BuildContext builder) {
          return Container(
            height: MediaQuery.of(context).copyWith().size.height / 3,
            color: Colors.white,
            child: CupertinoDatePicker(
              mode: CupertinoDatePickerMode.date,
              backgroundColor: Colors.yellow,
              onDateTimeChanged: (picked) {
                if (picked != null && picked != selectedDate)
                  setState(() {
                    selectedDate = picked;
                  });
              },
              initialDateTime: selectedDate,
              minimumYear: 2000,
              maximumYear: 2025,
            ),
          );
        });
  }

  buildMaterialDatePicker(BuildContext context) async {
    final DateTime picked = await showDatePicker(
      context: context,
      initialDate: selectedDate,
      firstDate: DateTime(2000),
      lastDate: DateTime(2025),
      initialEntryMode: DatePickerEntryMode.calendar,
      initialDatePickerMode: DatePickerMode.day,
      selectableDayPredicate: _decideWhichDayToEnable,
      helpText: 'Select event date',
      cancelText: 'Cancel',
      confirmText: 'Select',
      errorFormatText: 'Enter valid date',
      errorInvalidText: 'Enter date in valid range',
      fieldLabelText: 'Event date',
      fieldHintText: 'Month/Date/Year',
      builder: (context, child) {
        return Theme(
          data: ThemeData.light(),
          child: child,
        );
      },
    );
    if (picked != null && picked != selectedDate)
      setState(() {
        selectedDate = picked;
      });
  }

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------- FILTERING MENU OVERLAY -------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  showFilterOverlay  (BuildContext context) async {
    overlayState = Overlay.of(context);
    //_overlayEntry?.remove();
    _overlayEntry = OverlayEntry(
        builder: (context) => Positioned(
            top: 40,
            right: 3,
            child: Container(
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(20.0),
                boxShadow: [
                  BoxShadow(
                      color: Colors.grey.withOpacity(0.2),
                      spreadRadius: 3.0,
                      blurRadius: 5.0)
                ],
                color: Colors.white,),
              child: Column(
                children: [
                  /* -----------------  FILTER TITLE -------------------*/
                  Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Padding(
                          padding: EdgeInsets.only(top: 15.0, bottom: 10.0, left: 15, right: 15),
                          child:
                          Text('Filters',
                              style: TextStyle(
                                  fontFamily: 'Roboto',
                                  fontSize: 20.0,
                                  color: Colors.black,
                                  decoration: TextDecoration.none)
                          ),
                        ),
                      ]
                  ),
                  Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Padding(
                            padding: EdgeInsets.only(top: 5.0, bottom: 10.0, left: 15, right: 15),
                            child: Material (child: DropdownButton<String>(
                              value: dropdownValue,
                              icon: Icon(Icons.arrow_downward),
                              iconSize: 20,
                              elevation: 16,
                              style: TextStyle(fontFamily: 'Roboto', color: Colors.black),
                              underline: Container(
                                height: 2,
                                color: Colors.orange,
                              ),
                              onChanged: (String newValue) {
                                setState(() {
                                  dropdownValue = newValue;
                                });
                              },
                              items: <String>[' Alphabetical Asc.        ', 'Alphabetical Dsc.', 'Price Ascending', 'Price Descending']
                                  .map<DropdownMenuItem<String>>((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                            )
                            )
                        ),
                      ]
                  ),
                  /* ----------------------- PRICE SLIDER --------------------------------------------*/

                  Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Padding(
                          padding: EdgeInsets.only(top: 3.0, bottom: 1.0, left: 15, right: 15),
                          child:
                          Text('Max Price (€)',
                              style: TextStyle(
                                  fontFamily: 'Roboto',
                                  fontSize: 14.0,
                                  color: Colors.grey,
                                  decoration: TextDecoration.none)
                          ),
                        ),
                      ]
                  ),
                  Row(
                      mainAxisAlignment: MainAxisAlignment.start,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Material(

                            child: SliderTheme(
                                data: SliderTheme.of(context).copyWith(
                                  activeTrackColor: Colors.orange,
                                  inactiveTrackColor: Colors.grey,
                                  trackShape: RoundedRectSliderTrackShape(),
                                  trackHeight: 10.0,
                                  thumbColor: Colors.deepOrange,
                                  thumbShape: RoundSliderThumbShape(enabledThumbRadius: 12.0),
                                  overlayColor: Colors.red.withAlpha(32),
                                  overlayShape: RoundSliderOverlayShape(overlayRadius: 28.0),
                                ),
                                child: Slider(
                                  value: _currentPriceSliderValue,
                                  min: 0,
                                  max: 100,
                                  divisions: 5,
                                  label: _currentPriceSliderValue.round().toString(),
                                  onChanged: (double value) {
                                    setState(() {
                                      _currentPriceSliderValue = value;
                                    });
                                  },
                                )
                            )
                        )
                      ]
                  ),

                  /* ----------------------- CAPACITY SLIDER --------------------------------------------*/
                  Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Padding(
                          padding: EdgeInsets.only(top: 3.0, bottom: 1.0, left: 15, right: 15),
                          child:
                          Text('Capacity (no. of people)',
                              style: TextStyle(
                                  fontFamily: 'Roboto',
                                  fontSize: 14.0,
                                  color: Colors.grey,
                                  decoration: TextDecoration.none)
                          ),
                        ),
                      ]
                  ),
                  Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Material(

                            child: SliderTheme(
                                data: SliderTheme.of(context).copyWith(
                                  activeTrackColor: Colors.orange,
                                  inactiveTrackColor: Colors.grey,
                                  trackShape: RoundedRectSliderTrackShape(),
                                  trackHeight: 10.0,
                                  thumbColor: Colors.deepOrange,
                                  thumbShape: RoundSliderThumbShape(enabledThumbRadius: 12.0),
                                  overlayColor: Colors.red.withAlpha(32),
                                  overlayShape: RoundSliderOverlayShape(overlayRadius: 28.0),
                                ),
                                child: Slider(
                                  value: _currentCapacitySliderValue,
                                  min: 0,
                                  max: 20,
                                  divisions: 5,
                                  label: _currentCapacitySliderValue.round().toString(),
                                  onChanged: (double value) {
                                    setState(() {
                                      _currentCapacitySliderValue = value;
                                    });
                                  },
                                )
                            )
                        )
                      ]
                  ),
                  GestureDetector(
                    onTap: () {_overlayEntry.remove(); },
                    child:
                    Padding(
                      padding: EdgeInsets.all(15),
                      child:
                      Container(
                          decoration: BoxDecoration(
                            shape: BoxShape.rectangle,
                            borderRadius: BorderRadiusDirectional.all(
                                Radius.circular(25.0)),
                            color: Colors.deepOrange,
                          ),
                          child: Padding(
                            padding: EdgeInsets.only(top: 5, bottom: 5, left: 20, right: 20),
                            /* ------------------- Type orange tag -------------------*/
                            child: Text('Done', style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 18.0,
                                fontWeight: FontWeight.bold,
                                color: Colors.white,
                                decoration: TextDecoration.none)),
                          )
                      ),
                    ),
                  ),

                ],
              ),
            )
        ));

    overlayState.insert(_overlayEntry);
    //await Future.delayed(Duration(seconds: 15));
    //overlayEntry.remove();
  }


  /*---------------------------------- FUNCTIONS MATCH INTERFACE ----------------------------------*/
  Widget _buildCardForMatch(int id, String name, String description, String imagePath, String type, context) {
    final isFavorite = _favorites.contains(id);
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
                /*------------------- Image on the left -------------------*/
                Expanded(
                    child: Container(
                      //fit: BoxFit.contain,
                      alignment: Alignment.centerLeft,
                      child: Image.asset(imagePath),
                    )),
                /*------------------- Info on the right -------------------*/
                Expanded(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Padding(
                        padding: EdgeInsets.only(
                            left: 8.0, top: 8.0, bottom: 8.0, right: 8.0),
                        /* ------------------- Event Title -------------------*/
                        child: Text(name, style: TextStyle(
                            fontFamily: 'Roboto',
                            fontSize: 18.0,
                            fontWeight: FontWeight.bold,
                            color: Colors.deepOrange)),
                      ),
                      Padding(
                        padding: EdgeInsets.only(left: 8.0),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          //crossAxisAlignment: CrossAxisAlignment.start,
                          children: [

                            Container(
                                decoration: BoxDecoration(
                                    shape: BoxShape.rectangle,
                                    borderRadius: BorderRadiusDirectional.all(
                                        Radius.circular(20.0)),
                                    color: Colors.orange
                                ),
                                child: Padding(
                                  padding: EdgeInsets.all(6.0),
                                  /* ------------------- Type orange tag -------------------*/
                                  child: Text(type, style: TextStyle(
                                      fontFamily: 'Roboto',
                                      fontSize: 10.0,
                                      fontWeight: FontWeight.bold,
                                      color: Colors.white)),
                                )
                            ),
                            IconButton(
                              icon: Icon(
                                /*------------------- Favorite icon, with color and fill check -------------------*/
                                isFavorite ? Icons.favorite : Icons.favorite_border,
                                color: isFavorite ? Colors.red : null,
                              ),
                              onPressed: (){
                                setState(() {
                                  if(isFavorite){
                                    _favorites.remove(id);
                                  } else {
                                    _favorites.add(id);
                                  }
                                });
                              },
                            )
                            /*RaisedButton(
                                onPressed: () {pushEventDetails(id);},
                                color: Colors.orange,
                                textColor: Colors.white,
                                child: Padding(
                                  padding: EdgeInsets.all(6.0),
                                  /* ------------------- Type orange tag -------------------*/
                                  child: Text('Info', style: TextStyle(
                                      fontFamily: 'Roboto',
                                      fontSize: 5.0,
                                      fontWeight: FontWeight.bold,
                                      color: Colors.white)),
                                )
                            )*/

                          ],
                        ),
                      ),
                      Padding(
                        padding: EdgeInsets.only(
                            left: 50.0, top: 2.0, bottom: 2.0, right: 8.0),
                        /* ------------------- Event Title -------------------*/
                        child: Row(
                          children: [
                            FlatButton(
                              onPressed: () {pushEventDetails(id);},
                              color: Colors.orange,
                              textColor: Colors.white,
                              child: Text('Info', style: TextStyle(
                                    fontFamily: 'Roboto',
                                    fontSize: 15.0,
                                    fontWeight: FontWeight.bold,
                                    color: Colors.white)),
                              )

                        ],
                        )

                      ),
                      Padding(
                          padding: EdgeInsets.only(
                              left: 50.0, top: 2.0, bottom: 2.0, right: 8.0),
                          /* ------------------- Event Title -------------------*/
                          child: Row(
                            children: [
                              FlatButton(
                                onPressed: pushMatchMadeScreen,
                                color: Colors.deepOrange,
                                child: Text('MATCH', style: TextStyle(
                                    fontFamily: 'Roboto',
                                    fontSize: 15.0,
                                    fontWeight: FontWeight.bold,
                                    color: Colors.white)),
                              )
                            ],
                          )

                      )
                    ],
                  ),
                )
              ],
            )
        )
    );
  }

  Widget _buildMessage(int id, String name, String message, Color color, context) {
    return Padding(
        padding: EdgeInsets.only(left: 10.0, top: 5.0, right: 125.0, bottom: 5.0),
        child: Container(
            decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(15.0),
                boxShadow: [
                  BoxShadow(
                      color: Colors.white,
                      spreadRadius: 1.0)
                ],
                color: Colors.white),

            child: Row(
              children: <Widget>[
                /*------------------- Message -------------------*/
                Expanded(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      if(name != '')
                        Padding(
                          padding: EdgeInsets.only(
                              left: 10.0, top: 5.0, right: 20.0, bottom: 5.0),
                          /* ------------------- Event Title -------------------*/
                          child: Text(name, style: TextStyle(
                              fontFamily: 'Roboto',
                              fontSize: 15.0,
                              fontWeight: FontWeight.bold,
                              color: color)),
                        ),
                      Padding(
                        padding: EdgeInsets.only(
                            left: 10.0, top: 5.0, right: 20.0, bottom: 5.0),
                        child: Text(message, style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 15.0,
                                fontWeight: FontWeight.normal,
                                color: Colors.black)),
                      )

                    ],
                  ),
                )
              ],
            )
        )
    );
  }

  Widget _buildCardForFriend(int id, String name, bool friend, String image, String birthday, String email, String insta, String phone, String about,  context) {

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
                /*------------------- Info on the right -------------------*/

                Image.asset(image, height: 100, width: 100),

                Column(

                  children: [

                      Padding(

                          padding: EdgeInsets.only(left: 10.0, top: 10.0, bottom: 10.0),

                          child: Row(
                            children: [
                              Padding(
                                padding: EdgeInsets.only(left: 10.0, top: 5.0, bottom: 5.0),
                                child: Text(name,
                                    style: TextStyle(
                                        fontFamily: 'Roboto',
                                        fontSize: 18.0,
                                        color: friend ? Colors.deepOrange : Colors.black))

                              )]
                            )),

                      Container(

                                child: InkWell(
                                    onTap: () { pushProfile(id, name, birthday,
                                email, insta, phone,
                                about,
                                image); },
                                    child: Padding(
                                      padding: id == 2 ? EdgeInsets.all(6.0) : EdgeInsets.only(left: 26.0, top: 6.0, right: 6.0, bottom: 6.0),
                                  /* ------------------- Type orange tag -------------------*/
                                      child: FlatButton(
                                        onPressed: () { pushProfile(id, name, birthday,
                                            email, insta, phone,
                                            about,
                                            image); },
                                        color: Colors.deepOrange,
                                        child: Text('See profile', style: TextStyle(
                                            fontFamily: 'Roboto',
                                            fontSize: 15.0,
                                            fontWeight: FontWeight.bold,
                                            color: Colors.white)),
                                      )
                                )
    ) ) ] ),
                Column(
                  children: [
                    Padding(
                      padding: id == 2 ? EdgeInsets.only(left: 10.0) : EdgeInsets.only(left: 30.0),
                    child: IconButton(
                                  icon: Icon(
                                    /*------------------- Favorite icon, with color and fill check -------------------*/
                                    friend ? Icons.group : Icons.group_add,
                                    color: friend ? Colors.deepOrange : Colors.black,
                                    size: 35.0
                                  ),
                      onPressed: () { addFriend(id);  }
                    ),
                    ),
                    Padding(
                      padding: id == 2 ? EdgeInsets.only(left: 10.0) : EdgeInsets.only(left: 30.0),
                      child: Text( friend ? 'Friend' : 'Add Friend',
                                    style: TextStyle(
                                        fontFamily: 'Roboto',
                                        fontSize: 12.0,
                                        color: friend ? Colors.deepOrange : Colors.black))

                    )
                      ],
                        ),
                    ],
                  )
    )
    );
  }

  Widget _writeTextBar(int id) {
    return TextField(
        textAlign: TextAlign.left,
        textAlignVertical: TextAlignVertical.center,
        onSubmitted: (String value) async {
          sendMessage(value);
        },
        obscureText: false,
        decoration: InputDecoration(
          filled: true,
          fillColor: Colors.white,
          //border: round,
          labelText: 'Type a message' ),
      cursorWidth: 7.0,
      cursorHeight: 7.0
    );
  }

  Widget _buildGroupChatPreview() {
    return InkWell(
        onTap: pushChat,
        child: Padding(
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
                    /*------------------- Image on the left -------------------*/
                    Padding(
                      padding: EdgeInsets.only(left: 30.0),
                      child: Icon(Icons.group, color: Colors.deepOrange)
                    ),
                    /*------------------- Info on the right -------------------*/
                    Column(
                        mainAxisAlignment: MainAxisAlignment.start,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Padding(
                            padding: EdgeInsets.only(left: 20.0, top: 8.0),
                            child: Text('You, Beatriz Carvalho and Rodrigo Costa', style: TextStyle(
                                          fontFamily: 'Roboto',
                                          fontSize: 14.0,
                                          fontWeight: FontWeight.bold,
                                          color: Colors.deepOrange)),
                          ),
                          Padding(
                            padding: EdgeInsets.only(
                                left: 20.0, top: 8.0, bottom: 8.0, right: 8.0),
                            /* ------------------- Event Title -------------------*/
                            child: Text('Last message today', style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 12.0,
                                color: Colors.black)),
                          )
                        ],
                      ),
                  ],
                )
            )
        )
    );
  }

  void addFriend(int id) {
    id == 2 ?  BeatrizFriend = !BeatrizFriend  :  RodrigoFriend = !RodrigoFriend;
    pushAddFriendScreen();
  }

  void pushMatchScreenInterests() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {

          return Scaffold(
            appBar: AppBar(
              backgroundColor: Colors.orange,
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Match',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(

                padding: EdgeInsets.all(10.0),
                  children: [
                    /*Hard coded favorites check*/
                    Text('Events found based on your interests',
                        style: TextStyle(
                            fontFamily: 'Roboto',
                            fontSize: 15.0,
                            color: Colors.black)),
                    if(_favorites.contains(1))
                      _buildCardForMatch(
                          1,
                          'Listen (2020)',
                          'M14\nGenre: Drama \nDirector: Ana Rocha\n In the suburbs of London, Bela and Jota face serious challenges when Social Services raise doubts regarding the safety of their children. ',
                          'assets/listen.png',
                          'Cinema',
                          context),
                    if(_favorites.contains(2))
                      _buildCardForMatch(
                          2,
                          'NOS Alive',
                          'A music and arts festival which takes place in the Algés riverside in Oerias.\n\n Guests include Kendrick Lamar, Taylor Swift, Billie Eilish. Khalid, Alt-J and the Lumineers.\n\nCome have the time of your life!',
                          'assets/nos_alive.png',
                          'Concert',
                          context),
                    if(_favorites.contains(3))
                      _buildCardForMatch(
                          3,
                          'Pi 100 Pé',
                          'M06\n\nComedy show by Fernando Rocha.\nSit back, relax and have a laugh!\n\nSuper Bock Arena\n Campo Pequeno\nTickets available everywhere',
                          'assets/pi100pe.png',
                          'Comedy',
                          context),
                    if(_favorites.contains(4))
                      _buildCardForMatch(
                          4,
                          'Primeiro Modernismo às Novas Vanguardas do Século XX',
                          'The Berardo Collection brings together a remarkable of works that marked the history of art of the twentieth century',
                          'assets/berardo.png',
                          'Museum',
                          context),
                    if(_favorites.contains(5))
                      _buildCardForMatch(
                          5,
                          'Uma Mulher Não Chora',
                          'M06\nMusical\nFor 80 minutes, the singers join the musical universe of Renato Júnior. Paying homage to the reality of being a woman in the twenty first century, lending their voice to eradicate violence against women.',
                          'assets/teatro.png',
                          'Theater',
                          context),
                  ]
              ),


            floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
              backgroundColor: Colors.deepOrange,
              child: Image.asset('assets/groups.png', height: 35, width: 35,),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,

            bottomNavigationBar: _bottomAppBar(),


            );
        }, // ...to here.
      ),
    );
  }

  void pushProfile(int id, String name, String birthday, String email,
      String website, String number, String aboutMe, String image){
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {
          return Scaffold(
            backgroundColor: Colors.white70,
            appBar: AppBar(
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Profile',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(
                padding: EdgeInsets.only(top: 10.0),
                children: [
                  Column(
                    children: [
                      Container(
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
                          children: [
                            Image.asset(image, height: 150, width: 150),
                            Column(
                              children: [
                                    Padding(
                                      padding: EdgeInsets.all(5.0),
                                      child: Text(name, style: TextStyle(
                                          fontFamily: 'Roboto',
                                          fontSize: 16.0,
                                          fontWeight: FontWeight.bold,
                                          color: Colors.deepOrange)
                                      ),
                                    ),
                                    Row(
                                      children: [
                                        Padding(
                                          padding: EdgeInsets.all(5.0),
                                          child: Icon(Icons.cake, color: Colors.black),
                                        ),
                                        Padding(
                                          padding: EdgeInsets.all(5.0),
                                          child: Text(birthday, style: TextStyle(
                                              fontFamily: 'Roboto',
                                              fontSize: 14.0,
                                              color: Colors.black)
                                          ),
                                        ),
                                      ]
                                    ),
                                    Row(
                                        children: [
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Icon(Icons.email_outlined, color: Colors.black),
                                          ),
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Text(email, style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 14.0,
                                                color: Colors.black)
                                            ),
                                          ),
                                        ]
                                    ),
                                    Row(
                                        children: [
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Icon(Icons.alternate_email, color: Colors.black),
                                          ),
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Text(website, style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 14.0,
                                                color: Colors.black)
                                            ),
                                          ),
                                        ]
                                    ),
                                    Row(
                                        children: [
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Icon(Icons.phone, color: Colors.black),
                                          ),
                                          Padding(
                                            padding: EdgeInsets.all(5.0),
                                            child: Text(number, style: TextStyle(
                                                fontFamily: 'Roboto',
                                                fontSize: 14.0,
                                                color: Colors.black)
                                            ),
                                          ),
                                        ]
                                    ),
                                  ],
                                ),
                         ]
                      ),
                  ),

                  Padding(
                   padding: EdgeInsets.only(top: 10.0),
                   child:  Container(
                    decoration: BoxDecoration(
                       borderRadius: BorderRadius.circular(20.0),
                       boxShadow: [
                           BoxShadow(
                               color: Colors.grey.withOpacity(0.2),
                               spreadRadius: 3.0,
                               blurRadius: 5.0)
                       ],
                       color: Colors.white),
                       child: Column(
                        children: [
                             Padding(
                               padding: EdgeInsets.only(left: 15, top: 15, bottom: 8),
                               child: Text('About me', style: TextStyle(
                                    fontFamily: 'Roboto',
                                    fontSize: 16.0,
                                    fontWeight: FontWeight.bold,
                                    color: Colors.deepOrange)
                                ),
                             ),
                          Padding(
                            padding: EdgeInsets.only(left: 15, top: 15, bottom: 8),
                            child: Text(aboutMe, style: TextStyle(
                                fontFamily: 'Roboto',
                                fontSize: 14.0,
                                color: Colors.black)
                            ),
                          ),
                        ],
                     ),
                   ),
                  ),

                  if(id == 1)
                    Padding(
                      padding: EdgeInsets.only(top: 10.0),
                      child:  Container(
                        width:390,
                          decoration: BoxDecoration(
                              borderRadius: BorderRadius.circular(20.0),
                              boxShadow: [
                                BoxShadow(
                                   color: Colors.grey.withOpacity(0.2),
                                   spreadRadius: 3.0,
                                   blurRadius: 5.0)
                              ],
                              color: Colors.white),
                          child: Column(
                             children: [
                               Padding(
                                 padding: EdgeInsets.only(left: 15, top: 15, bottom: 8),
                                  child: Text('Preferred payment method', style: TextStyle(
                                      fontFamily: 'Roboto',
                                      fontSize: 16.0,
                                      fontWeight: FontWeight.bold,
                                      color: Colors.deepOrange)
                                  ),
                               ),
                               Padding(
                                 padding: EdgeInsets.only(left: 15, top: 15, bottom: 8),
                                    child: Text('Visa ending in **39', style: TextStyle(
                                        fontFamily: 'Roboto',
                                        fontSize: 12.0,
                                        color: Colors.black),
                                    ),
                                  ),
                                ],
                              ),

                            )
          )

                   ]
                  )
                ]
            ),
            floatingActionButton: FloatingActionButton(onPressed: pushChat,
              backgroundColor: Colors.deepOrange,
              child: Icon(Icons.edit, size: 35, color: Colors.white),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,
          );
        }, // ...to here.
      ),
    );
  }

  void pushMatchMadeScreen() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {
          return Scaffold(
            backgroundColor: Colors.orange,
            appBar: AppBar(
              elevation: .0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Successful match!',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(
              //padding: EdgeInsets.only(top: 30.0),
                children: <Widget>[
                  Expanded(
                      child: Container(
                        //fit: BoxFit.contain,
                        alignment: Alignment.centerLeft,
                        child: Image.asset('assets/match.PNG'),
                      )
                  ),
                  /*
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      crossAxisAlignment: CrossAxisAlignment.center,
                      children: [
                        Text("No reviews yet", style: TextStyle(fontSize: 30, color: Colors.grey, fontFamily: 'Roboto'),),
                      ],
                    ),
                     */
                ]),
                floatingActionButton: FloatingActionButton(onPressed: () { hasGroupChat = true; pushChat(); },
                  backgroundColor: Colors.white,
                  child: Icon(Icons.forum, size: 35, color: Colors.deepOrange),
                ),
                floatingActionButtonLocation: FloatingActionButtonLocation.centerFloat
          );
        }, // ...to here.
      ),
    );
  }

  void pushChat() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {

          return Scaffold(
            backgroundColor: Colors.grey[50],
            appBar: AppBar(
              backgroundColor: Colors.orange,
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Chat',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
              actions: <Widget>[
                IconButton(
                    icon: Icon( Icons.group_add, color: Colors.white ),
                    onPressed: pushAddFriendScreen
                ),
              ]
            ),
            body: ListView(

                padding: EdgeInsets.all(10.0),
                children: [

                  /*Messages*/
                  _buildMessage(
                        1,
                        'Rodrigo Costa',
                        'Mal posso esperar por este dia!',
                        Colors.green,
                        context),
                  _buildMessage(
                      2,
                      '',
                      'Queria bué ir a isto, mas não tinha ninguém com quem ir...',
                      Colors.green,
                      context),
                  _buildMessage(
                      3,
                      'Beatriz Carvalho',
                      'A que horas combinamos encontrar-nos?',
                      Colors.blue,
                      context),
                  for(int i = 0; i < _myMessages.length; i++)
                    _buildMessage(
                        4 + i,
                        i == 0 ? 'Me' : '',
                        _myMessages[i],
                        Colors.deepOrange,
                        context),

                  Padding(
                      padding: EdgeInsets.only(top: 300),
                      child: _writeTextBar(1)
                  )

                ],
            ),

            floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
              backgroundColor: Colors.deepOrange,
              child: Image.asset('assets/groups.png', height: 35, width: 35),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,
            bottomNavigationBar: _bottomAppBar(),

          );
        }, // ...to here.
      ),
    );
  }

  void sendMessage(String message) {
    _myMessages.add(message);
    pushChat();
  }

  void pushAddFriendScreen() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {

          return Scaffold(
            backgroundColor: Colors.white,
            appBar: AppBar(
              backgroundColor: Colors.orange,
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Add Friend',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(
                padding: EdgeInsets.all(10.0),
                children: [
                 _buildCardForFriend(
                        2,
                        'Beatriz Carvalho, 21',
                        BeatrizFriend,
                        'assets/beatriz.png',
                        'April 29th 1999',
                        'beatriz99@sapo.pt',
                        'twitter.com/beacarvalho99',
                        '938972365',
                        'Hello, I’m an University student who likes to watch movies, listen to music and hang out with my friends.',
                        context),
                  _buildCardForFriend(
                      3,
                      'Rodrigo Costa, 24',
                      RodrigoFriend,
                      'assets/rodrigo.png',
                      'May 3rd 1996',
                      'rodrigo@hotmail.com',
                      'instagram.com/rodrigoc96',
                      '932347622',
                      'Hi. I’m an University student from Lisbon. I like to play videogames, go to the cinema and listening to music. I’m here to find some people to go to some interesting events.',
                      context),
                ]
            ),
            floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
              backgroundColor: Colors.deepOrange,
              child: Image.asset('assets/groups.png', height: 35, width: 35,),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,

            bottomNavigationBar: _bottomAppBar(),

          );
        }, // ...to here.
      ),
    );
  }

  void pushGroupChats() {
    Navigator.of(context).push(
      MaterialPageRoute<void>(
        // Add lines from here...
        builder: (BuildContext context) {


          return Scaffold(
            appBar: AppBar(
              backgroundColor: Colors.orange,
              elevation: 9.0,
              leading: IconButton(
                icon: Icon(Icons.arrow_back, color: Colors.white),
                onPressed: () {
                  Navigator.pop(context);
                },
              ),
              title: Text('Group Chats',
                  style: TextStyle(
                      fontFamily: 'Roboto',
                      fontSize: 20.0,
                      color: Colors.white)),
            ),
            body: ListView(
                padding: EdgeInsets.all(10.0),
                children: [
                  /*Hard coded favorites check*/
                  if(hasGroupChat)
                    _buildGroupChatPreview(),
                  if(!hasGroupChat)
                    Padding(
                      padding: EdgeInsets.only(
                        left: 8.0, top: 8.0, bottom: 8.0, right: 8.0),
                      child: Text('You are not in any group chat yet', style: TextStyle(
                        fontFamily: 'Roboto',
                        fontSize: 18.0,
                        color: Colors.black)),
                    ),
                ]
            ),
            floatingActionButton: FloatingActionButton(onPressed: pushMatchScreenInterests,
              backgroundColor: Colors.deepOrange,
              child: Image.asset('assets/groups.png', height: 35, width: 35),
            ),
            floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,

            bottomNavigationBar: _bottomAppBar(),

          );
        }, // ...to here.
      ),
    );
  }

}
