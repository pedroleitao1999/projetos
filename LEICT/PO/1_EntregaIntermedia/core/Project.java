package sth.core;

import java.util.Collection;
import java.util.ArrayList;

public class Project{
	private String _name;
	private String _description;
	private boolean _closed;

	public Project(String name, String description, boolean closed){
		_name = name;
		_description = description;
		_closed = closed;
	}

	public Project(String name, boolean closed){
		_name = name;
		_closed = closed;
	}

	public String getName(){
		return _name;
	}

	void close(){
		_closed = true;
	}
}
