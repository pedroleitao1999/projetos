import 'package:flutter/material.dart';
import './loginPage.dart';
import './signUpPage.dart';

class FirstPage extends StatelessWidget {

  Widget logo() {
   return Stack(
       children: <Widget>[Positioned(
       top: 200,
       left: 0,
       right: 0,
       child: Container(
         width: 250,
         height:250,
         decoration: BoxDecoration(
             image: DecorationImage(image: AssetImage('assets/firstPageLogo.png'), scale: 2)
     ),
   ))]);
  }

  Widget topDeco() {
    return Stack(
        children: <Widget>[Positioned(
            top: -35.0,
            right: 141.0,
            child: Container(
                width: 250,
                height:250,
                decoration: BoxDecoration(
                  image: DecorationImage(image: AssetImage('assets/topDecoration.png'), scale: 2)
                ),
              ))]);
  }

  Widget botDeco() {
    return Stack(
        children: <Widget>[Positioned(
            bottom: 0.0,
            left: 141.0,
            child: Container(
              width: 250,
              height:250,
              decoration: BoxDecoration(
                  image: DecorationImage(image: AssetImage('assets/BotDecoration.png'), scale: 2)
              ),
            ))]);
  }

  @override
  Widget build(BuildContext context) {
    return Container(
        decoration: BoxDecoration(
        gradient: LinearGradient(
        begin: Alignment.topLeft,
        end: Alignment.bottomRight,
        stops: [0.1, 0.3, 0.5, 0.7, 0.9],
        colors: [
          Colors.orange,
        Colors.orange[600],
        Colors.orange[700],
        Colors.orange[800],
        Colors.orange[900],
        ],
    )),
    child: Stack(
      children: <Widget>[logo(), topDeco(), botDeco(),
        new Positioned.fill(
            top: 190,
            child: Align(
              alignment: Alignment.center,
              child: FlatButton(
                color: Colors.white,
                textColor: Colors.orange,
                padding: EdgeInsets.all(8.0),
                minWidth: 100,
                shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                onPressed: () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => LoginPage()),
                  );
                },
                child: Text(
                  "Log In",
                  style: TextStyle(fontSize: 20.0, fontFamily:'Roboto', color: Colors.deepOrange),
                )))),
        new Positioned.fill(
            top: 305,
            child: Align(
              alignment: Alignment.center,
              child: FlatButton(
                color: Colors.white,
                textColor: Colors.orange,
                padding: EdgeInsets.all(8.0),
                minWidth: 100,
                shape: new RoundedRectangleBorder(borderRadius: new BorderRadius.circular(30.0)),
                onPressed: () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => SignUpPage()),
                  );
                },
                child: Text(
                  "Sign Up",
                  style: TextStyle(fontSize: 20.0, fontFamily:'Roboto', color: Colors.deepOrange),
                ),
              )))]));
  }
}