import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:culturally/main.dart';
import 'package:path/path.dart';
//import 'package:sqflite/sqflite.dart';

class SignUpPage extends StatefulWidget {
  @override
  SignUpPageState createState() => SignUpPageState();
}

class SignUpPageState extends State<SignUpPage> {

  // ignore: non_constant_identifier_names
  Widget Deco() {
    return Stack(
        children: <Widget>[Positioned(
            top: 0.0,
            left: 152.0,
            child: Container(
              width: 250,
              height:250,
              decoration: BoxDecoration(
                  image: DecorationImage(image: AssetImage('assets/loginDecoration.png'), scale: 2)
              ),
            ))]);
  }

  @override
  Widget build(BuildContext context) {
    return Container(
        color: Colors.white,
        child: Stack(
            children: <Widget>[
              Scaffold(
                  resizeToAvoidBottomInset: false,
                  extendBodyBehindAppBar: true,
                  backgroundColor: Colors.transparent,
                  appBar: new AppBar(
                      primary: true,
                      iconTheme: IconThemeData(
                        color: Colors.deepOrange,
                      ),
                      backgroundColor: Colors.transparent,
                      elevation: 0),
                  body: Container(
                      child: Column(
                        children: <Widget>[
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: <Widget>[
                                Container(
                                    width: 340,
                                    padding: EdgeInsets.only(top:280),
                                    child: TextField(
                                        textAlign: TextAlign.left,
                                        textAlignVertical: TextAlignVertical.center,
                                        obscureText: false,
                                        decoration: InputDecoration(
                                          filled: true,
                                          fillColor: Colors.grey[300],
                                          border: OutlineInputBorder(),
                                          labelText: 'Your name*',
                                        )))]),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: <Widget>[
                                Container(
                                    width: 340,
                                    padding: EdgeInsets.only(top:15),
                                    child: TextField(
                                        textAlign: TextAlign.left,
                                        textAlignVertical: TextAlignVertical.center,
                                        obscureText: false,
                                        decoration: InputDecoration(
                                          filled: true,
                                          fillColor: Colors.grey[300],
                                          border: OutlineInputBorder(),
                                          labelText: 'Email address*',
                                        )))]),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: <Widget>[
                                Container(
                                    width: 340,
                                    padding: EdgeInsets.only(top:15),
                                    child: TextField(
                                        textAlign: TextAlign.left,
                                        textAlignVertical: TextAlignVertical.center,
                                        obscureText: true,
                                        decoration: InputDecoration(
                                          filled: true,
                                          fillColor: Colors.grey[300],
                                          border: OutlineInputBorder(),
                                          labelText: 'Password*',
                                        )))]),
                          Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: <Widget>[
                                Container(
                                    width: 340,
                                    padding: EdgeInsets.only(top:15),
                                    child: TextField(
                                        textAlign: TextAlign.left,
                                        textAlignVertical: TextAlignVertical.center,
                                        obscureText: true,
                                        decoration: InputDecoration(
                                          filled: true,
                                          fillColor: Colors.grey[300],
                                          border: OutlineInputBorder(),
                                          labelText: 'Confirm password',
                                        )))]),
                          Row(
                              children: <Widget>[
                                Container(
                                    padding: EdgeInsets.only(top:15),
                                    child: Text('     *Required'
                                        ))])])),
                  ), Deco(),
              new Positioned.fill(
                  top: -300,
                  child: Align(
                      alignment: Alignment.center,
                      child: Text('Sign Up',
                          style: TextStyle(fontSize: 36, fontWeight: FontWeight.normal, fontFamily:'Roboto', color: Colors.deepOrange, decoration: TextDecoration.none)))),
              new Positioned.fill(
                  top: 550,
                  child: Align(
                      alignment: Alignment.center,
                      child: FlatButton(
                        color: Colors.orange[700],
                        textColor: Colors.white,
                        padding: EdgeInsets.all(8.0),
                        minWidth: 100,
                        shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                        onPressed: () {
                          Navigator.of(context).push(
                            MaterialPageRoute<void>(
                                builder: (BuildContext context) {
                                  return HomePage();
                                }
                            ),
                          );
                        },
                        child: Text(
                          "Sign Up",
                          style: TextStyle(fontSize: 20.0, fontFamily: 'Roboto'),
                        ),
                      )))
            ]));
  }
}