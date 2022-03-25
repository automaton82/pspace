//David Millman

#ifndef _TEXTTABLE_H_
#define _TEXTTABLE_H_

#include "DataTypes.h"

#include "AsciiUtil.h"
#include "GameObject.h"
#include "TextureFont.h"

using namespace AsciiUtil;

class TextTable : 
	public GameObject
{
public:
	TextTable();
	TextTable(Uint rows, Uint columns);
	virtual ~TextTable();

	//settings
	void setFont(const TextureFont& font);

	void setWidth(Uint width);				//width and height based on font size, i.e. 1 width = 1 letter width
	void setHeight(Uint height);
	
	Uint numRows() const;
	Uint numColumns() const;

	//add text	//note: color corresponds to a font set color (in the bmp)
	void setColumnHeader(Uint columnId, const string& header, CenterType centerType = leftJustified, int color = 0);
	void setCell(Uint rowId, Uint columnId, const string& value, CenterType centerType = unknownCenterType, int color = 0);

	//cell manipulation
	void setRowHeight(Uint rowHeight);
	void setRowHeight(Uint rowId, Uint rowHeight);
	void setColumnWidth(Uint columnId, Uint columnWidth);

	void addRow(Uint rowHeight = 1);
			//adds first row if it doesn't exist
	void addColumn(Uint columnWidth = 1, const string& header = "", CenterType centerType = leftJustified, int color = 0);
		

	//void clear();

	//display
	void scrollDown(Uint n = 1);		//TODO: use ints - reduce to 2 functions?
	void scrollUp(Uint n = 1);
	void scrollRight(Uint n = 1);
	void scrollLeft(Uint n = 1);

	void setBackgroundColor(const Color& c);	//includes only background transparency
	void setPadding(double left, double right, double top, double bottom);
	void setTransparency(double a);				//overall transparency
	
	//void cacheTable();		//clamp all strings into actual [x][y] cells to 'save' the current table

	void draw() const;

protected:

	struct ColumnCell
	{
		ColumnCell() : color(0), centerType(leftJustified), width(0)
		{}

		string value;
		int color;
		CenterType centerType;

		Uint width;
	};

	typedef vector<ColumnCell> ColumnList;	//TODO: the cells could be pointers for better performance

	struct RowCell
	{
		RowCell() : height(0)
		{}

		ColumnList columns;

		Uint height;
	};

	typedef vector<RowCell> RowList;

protected:

	ColumnList columnHeaders_;
	RowList rows_;
	Uint rowWidth_;
	Uint columnHeight_;
	
	Color backgroundColor_;
	double transparency_;
	double padLeft_, padRight_, padTop_, padBottom_;

	Uint numRows_, numCols_;

	Uint currentX_, currentY_;			//current display position, (0,0) at top left
	Uint displayHeight_, displayWidth_;	//height and width in terms of characters

	TextureFont font_;
};

#endif