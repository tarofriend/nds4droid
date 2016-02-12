package com.opendoorstudios.ds4droid.NDSScanner;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

//==============================================================================
public class RomDatabaseHelper extends SQLiteOpenHelper {
	//--------------------------------------------------------------------------
	
	public static final String DB_NAME    = "roms.db";
	public static final int    DB_VERSION = 1;
	
	//--------------------------------------------------------------------------
	
	public RomDatabaseHelper( Context context ) {
		super( context, DB_NAME, null, DB_VERSION );
	}

	//--------------------------------------------------------------------------

	@Override
	public void onCreate( SQLiteDatabase db ) {
		db.execSQL(
			"CREATE TABLE roms            ( " +
				"path     TEXT PRIMARY KEY, " +
				"title    TEXT            , " +
				"gamecode TEXT              " +
			");" );
	}

	//--------------------------------------------------------------------------
	
	@Override
	public void onUpgrade( SQLiteDatabase db, int old_version, int new_version ) {
		switch ( old_version ) {
		}	
	}
	
	//--------------------------------------------------------------------------
}
//------------------------------------------------------------------------------