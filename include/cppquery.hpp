#pragma once

#ifndef __EMSCRIPTEN__
	#error Emscripten required.
#endif

#include <queue>
#include <string>

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

namespace cppquery {
	void initialize( void ) {};

	namespace http {
		void initialize( void ) {};
	};

	namespace html {
		void initialize( void ) {};

		extern "C" {
			void createElement( const char* elementType, const char* parentQuery = "body", const char* idName = NULL ) {
				EM_ASM( {
					newElement = document.createElement( UTF8ToString( $0 ) );
					document.querySelector( UTF8ToString( $1 ) ).appendChild( newElement );

					if ( $2 !== 0 )
						newElement.setAttribute( "id", UTF8ToString( $2 ) );
				}, elementType, parentQuery, idName );
			};

			void createElementText( const char* parentElementId, const char* text ) {
				EM_ASM( {
					newElement = document.createTextNode( UTF8ToString( $1 ) );
					document.querySelector( UTF8ToString( $0 ) ).append( newElement );
				}, parentElementId, text );
			};

			void setElementId( const char* query, const char* idName ) {
				EM_ASM( {
					document.querySelector( UTF8ToString( $0 ) ).setAttribute( "id", UTF8ToString( $1 ) );
				}, query, idName );
			};

			void addElementClass( const char* query, const char* className ) {
				EM_ASM( {
					document.querySelector( UTF8ToString( $0 ) ).classList.add( UTF8ToString( $1 ) );
				}, query, className );
			};

			void removeElementClass( const char* query, const char* className ) {
				EM_ASM( {
					document.querySelector( UTF8ToString( $0 ) ).classList.remove( UTF8ToString( $1 ) );
				}, query, className );
			};

			void appendElement( const char* elementName, const char* query ) {
				EM_ASM( {
					document.querySelector( UTF8ToString( $1 ) ).appendChild( document.querySelector( UTF8ToString( $0 ) ) );
				}, elementName, query );
			};
		};

		int getBodyWidth( void ) {
			return EM_ASM_INT( {
				return document.body.clientWidth;
			} );
		};

		int getBodyHeight( void ) {
			return EM_ASM_INT( {
				return document.body.clientHeight;
			} );
		};
	};

	namespace websocket {
		enum STATES {
			CONNECTING = 0,
			OPENED,
			CLOSING,
			CLOSED
		};

		void initialize( void ) {
			EM_ASM( {
				javascriptWebsocketSetMessageInboundMessage = Module.cwrap( "cppWebsocketSetMessageInboundMessage", null, "string" );
			} );
		};

		std::queue< const std::string > incomingMessages;

		EM_JS( void, javascriptWebsocketSetMessageOutbound, ( const char* message ), {
			socket.send( UTF8ToString( message ) );
		} );

		extern "C" {
			EMSCRIPTEN_KEEPALIVE
			void cppWebsocketSetMessageInboundMessage( const char* message ) {
				incomingMessages.push( message );
			};
		};

		int getState( void ) {
			return EM_ASM_INT( {
				return socket.readyState;
			} );
		};

		EM_JS( void, openSocket, ( const char* path ), {
			socket = new WebSocket( UTF8ToString( path ) );

			socket.addEventListener( "message", ( message ) => {
				const length = lengthBytesUTF8( message.data ) + 1;
				const cString = _malloc( length );
				stringToUTF8( message.data, cString, length );
				javascriptWebsocketSetMessageInboundMessage( cString );
				_free( cString );
			} );
		} );

		EM_JS( void, closeSocket, (), {
			socket.close();

			delete socket;
		} );

		std::size_t messagesCount( void ) {
			return incomingMessages.size();
		};

		std::string getMessage( void ) {
			const std::string message = incomingMessages.front();
			incomingMessages.pop();

			return message;
		};

		void sendMessage( const std::string data ) {
			javascriptWebsocketSetMessageOutbound( data.c_str() );
		};
	};
};
