// var Module = {
// 	totalDependencies: 0,

// 	print: ( message ) => {
// 		console.log( message );
// 	},

// 	printErr: ( message ) => {
// 		console.error( message );
// 	},

// 	setStatus: ( text ) => {
// 		if ( ! Module.setStatus.last )
// 			Module.setStatus.last = {
// 				time:	Date.now(),
// 				text:	""
// 			};

// 		if ( text === Module.setStatus.last.text )
// 			return;

// 		const m = text.match( /([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/ );
// 		const now = Date.now();

// 		if ( m && now - Module.setStatus.last.time < 30 )
// 			return;

// 		Module.setStatus.last.time = now;
// 		Module.setStatus.last.text = text;

// 		if ( m )
// 			text = m[ 1 ];
// 	},

// 	monitorRunDependencies: ( left ) => {
// 		this.totalDependencies = Math.max( this.totalDependencies, left );

// 		Module.setStatus( left ? `Preparing... (${ this.totalDependencies - left }/${ this.totalDependencies })` : "All downloads complete." );
// 	}

// 	__javascriptElements: = {};
// };

// window.onerror = ( event ) => {
// 	Module.setStatus( "Exception thrown. See JavaScript console." );

// 	Module.setStatus = ( text ) => {
// 		if ( text )
// 			Module.printErr( `[post-exception status] ${ text }` );
// 	};
// };

// Module.setStatus( "Downloading..." );
