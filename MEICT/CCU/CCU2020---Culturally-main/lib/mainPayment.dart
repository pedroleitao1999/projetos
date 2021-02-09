import 'package:culturally/MbwayPayment.dart';
import 'package:culturally/PaypalPayment.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:culturally/main.dart';
import './loginPage.dart';
import './signUpPage.dart';
import './visaPayment.dart';
import './PaypalPayment.dart';
import './MultibancoPayment.dart';
import './MbwayPayment.dart';
import './main.dart';

class mainPayment extends StatelessWidget {
  @override
  Widget build(BuildContext context) {

    void pushHomepage(){
      Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return HomePage();
            }
        ),
      );
    }

    void pushVisaPayment(){
      Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return VisaPayment();
            }
        ),
      );
    }

    void pushMbwayPayment(){
      Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return MbWayPayment();
            }
        ),
      );
    }

    void pushPaypalPayment(){
      Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return PaypalPayment();
            }
        ),
      );
    }

    void pushMultibancoPayment(){
      Navigator.of(context).push(
        MaterialPageRoute<void>(
            builder: (BuildContext context) {
              return MultibancoPayment();
            }
        ),
      );
    }

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
            title: Text('Payment',
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
                                  onPressed: pushHomepage),
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
        body: Column(
          children: [Row(
            children: <Widget>[
              Padding(
                  padding: EdgeInsets.only(left:40, right:10),
                  child : InkWell(
                      onTap: pushVisaPayment,
                      child: Container(
                        width: 60,
                        height:60,
                        decoration: BoxDecoration(
                          image: DecorationImage(image: AssetImage('assets/visa.png'), scale: 1)
                      ))),
                    ),
              Padding(
                  padding: EdgeInsets.all(10.0),
                  child : InkWell(
                    onTap: pushMbwayPayment,
                    child: Container(
                      width: 60,
                      height:60,
                      decoration: BoxDecoration(
                        image: DecorationImage(image: AssetImage('assets/mbway.png'), scale: 2)
                      ),
                  ))),
              Padding(
                  padding: EdgeInsets.all(10.0),
                  child : InkWell(
                    onTap: pushPaypalPayment,
                    child: Container(
                      width: 60,
                      height:60,
                      decoration: BoxDecoration(
                        image: DecorationImage(image: AssetImage('assets/paypal.png'), scale: 2)
                      ),
                      ))),
              Padding(
                  padding: EdgeInsets.all(10.0),
                  child : InkWell(
                    onTap: pushMultibancoPayment,
                    child: Container(
                       width: 60,
                       height:60,
                       decoration: BoxDecoration(
                          image: DecorationImage(image: AssetImage('assets/multibanco.png'), scale: 2)
                      ),
                  )))]),
              new Container(
                    padding: EdgeInsets.only(bottom:380, right:10),
                    child: Text(
                    "Select a payment method above",
                    style: TextStyle(fontSize: 18.0),
                  ),
            )]));
  }
}