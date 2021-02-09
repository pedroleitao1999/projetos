import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:culturally/main.dart';

class LoginPage extends StatefulWidget {
  @override
  LoginPageState createState() => LoginPageState();
}

class LoginPageState extends State<LoginPage> {

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
          extendBodyBehindAppBar: true,
          backgroundColor: Colors.transparent,
          appBar: new AppBar(
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
                          padding: EdgeInsets.only(top:300),
                          child: TextField(
                            textAlign: TextAlign.left,
                            textAlignVertical: TextAlignVertical.center,
                            obscureText: false,
                            decoration: InputDecoration(
                              filled: true,
                              fillColor: Colors.grey[300],
                            border: OutlineInputBorder(),
                            labelText: 'Email address',
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
                                    labelText: 'Password',
                                  )))])]),
                )), Deco(),
        new Positioned.fill(
            top: -250,
            child: Align(
              alignment: Alignment.center,
              child: Text('Log In',
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
                    "Log In",
                    style: TextStyle(fontSize: 20.0),
                  ),
                )))
        ]));
  }
}