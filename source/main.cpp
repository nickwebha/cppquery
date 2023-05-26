#include <iostream>

#include <emscripten.h>

#include "../include/cppquery.hpp"

int main( void ) {
	cppquery::initialize();
	cppquery::http::initialize();
	cppquery::html::initialize();
	cppquery::css::initialize();
	cppquery::websocket::initialize();

	// cppquery::http::fetch( "http://localhost/" );

	cppquery::html::createElement( "div", "body", "main" );
	cppquery::html::createElement( "p", "body", "someValue" );
	cppquery::html::createElementText( "#someValue", "Hello, world!" );
	cppquery::html::appendElement( "#main", "#someValue" );
	cppquery::html::setElementId( "#someValue", "xyz" );
	cppquery::html::addElementClass( "#xyz", "someClass1" );
	cppquery::html::addElementClass( "#xyz", "someClass2" );
	cppquery::html::removeElementClass( "#xyz", "someClass1" );

	cppquery::websocket::openSocket( "ws://localhost:8083/" );

	while ( true ) {
		const auto websocketState = cppquery::websocket::getState();
		if ( websocketState == cppquery::websocket::STATE::OPENED )
			cppquery::websocket::sendMessage( "Hello, world!" );
		else if ( websocketState == cppquery::websocket::STATE::CLOSING || websocketState == cppquery::websocket::CLOSED )
			break;

		while ( cppquery::websocket::messagesCount() > 0 )
			std::cout << cppquery::websocket::getMessage() << std::endl;

		emscripten_sleep( 1000 );
	}

	cppquery::websocket::closeSocket();

	return 0;
};
