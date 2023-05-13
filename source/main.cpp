#include <iostream>
#include <unistd.h>

#include <emscripten.h>

#include "../include/cppquery.hpp"

int main( void ) {
	cppquery::initialize();
	cppquery::http::initialize();
	cppquery::html::initialize();
	cppquery::websocket::initialize();

	cppquery::html::createElement( "div", "body", "main" );
	cppquery::html::addElementClass( "#main", "someClass1" );
	cppquery::html::addElementClass( "#main", "someClass2" );
	cppquery::html::removeElementClass( "#main", "someClass1" );

	cppquery::html::createElement( "p", "body", "someValue" );
	cppquery::html::createElementText( "#someValue", "Nothing to see here." );
	cppquery::html::appendElement( "#someValue", "#main" );
	cppquery::html::setElementId( "#someValue", "xyz" );

	cppquery::websocket::openSocket( "ws://localhost:8083/" );
	while ( true ) {
		const auto websocketState = cppquery::websocket::getState();

		if ( websocketState == cppquery::websocket::STATES::OPENED )
			cppquery::websocket::sendMessage( "Hello, world!" );
		else if ( websocketState == cppquery::websocket::STATES::CLOSING || websocketState == cppquery::websocket::CLOSED )
			break;

		while ( cppquery::websocket::messagesCount() > 0 )
			std::cout << cppquery::websocket::getMessage() << std::endl;

		emscripten_sleep( 1000 );
	}

	cppquery::websocket::closeSocket();

	return 0;
};
