//David Millman

#include "TextTable.h"

TextTable::TextTable() :
	numRows_(0), numCols_(0), displayWidth_(0), displayHeight_(0), rowWidth_(0), columnHeight_(0), 
	currentX_(0), currentY_(0),
	padLeft_(1), padRight_(1), padTop_(1), padBottom_(1),
	transparency_(1)
{
}

TextTable::TextTable(Uint rows, Uint columns) :
	numRows_(rows), numCols_(columns), displayWidth_(0), displayHeight_(0), rowWidth_(0), columnHeight_(0),
	currentX_(0), currentY_(0),
	padLeft_(1), padRight_(1), padTop_(1), padBottom_(1),
	transparency_(1)
{
	rows_.resize(numRows_);

	columnHeaders_.resize(numCols_);
	RowList::iterator i;
	for(i = rows_.begin(); i != rows_.end(); ++i)
	{
		(*i).columns.resize(numCols_);
	}
}

TextTable::~TextTable()
{
}

void TextTable::setFont(const TextureFont& font)
{
	font_ = font;

	if(!displayHeight_)
		displayHeight_ = (numRows_+1) + padTop_ + padBottom_;

	if(!displayWidth_)
	{
		Uint width = 0;
		
		if(rows_.size() > 0)
		{
			ColumnList::iterator i;
			for(i = rows_[0].columns.begin(); i != rows_[0].columns.end(); ++i)
			{
				width += (*i).width;
			}
		}

		displayWidth_ = width + padLeft_ + padRight_;
	}

}

void TextTable::setWidth(Uint width)
{
	displayWidth_ = width;
}

void TextTable::setHeight(Uint height)
{
	displayHeight_ = height;
}

Uint TextTable::numRows() const
{
	return numRows_;
}

Uint TextTable::numColumns() const
{
	return numCols_;
}

void TextTable::setColumnHeader(Uint columnId, const string& header, CenterType centerType, int color)
{
	assert(columnHeaders_.size() > columnId);

	ColumnCell& cell = columnHeaders_[columnId];
	cell.value = header;
	cell.color = color;
	cell.centerType = centerType;

	if(rows_.size() > 0 && rows_[0].columns.size() > 0)
	{
        cell.width = rows_[0].columns[0].width;
	}
}

void TextTable::setCell(Uint rowId, Uint columnId, const string& value, CenterType centerType, int color)
{
	assert(rows_.size() > rowId);
	assert(rows_[rowId].columns.size() > columnId);

	ColumnCell& cell = rows_[rowId].columns[columnId];
	cell.value = value;
	cell.color = color;

	if(centerType != unknownCenterType)
		cell.centerType = centerType;
}

void TextTable::setRowHeight(Uint rowHeight)
{
	RowList::iterator i;
	for(i = rows_.begin(); i != rows_.end(); ++i)
	{
		(*i).height = rowHeight;
	}
}

void TextTable::setRowHeight(Uint rowId, Uint rowHeight)
{
	assert(rows_.size() > rowId);

	columnHeight_ += rowHeight - rows_[rowId].height;

	rows_[rowId].height = rowHeight;
}

void TextTable::setColumnWidth(Uint columnId, Uint columnWidth)
{
	assert(columnId < numCols_);

	assert(rows_.size() > 0);
	
	rowWidth_ += columnWidth - rows_[0].columns[columnId].width;
	
	RowList::iterator i;
	for(i = rows_.begin(); i != rows_.end(); ++i)
	{
		(*i).columns[columnId].width = columnWidth;
	}
}

void TextTable::addRow(Uint rowHeight)
{
	RowCell newCell;

	//newCell.columns.resize(numCols_);
	newCell.height = rowHeight;

	if(rows_.size() > 0)
	{
		ColumnList& cols = rows_[0].columns;

		for(Uint i = 0; i < cols.size(); ++i)
		{
			ColumnCell newCol = cols[i];
			newCol.value = "-EMPTY-";
			
			newCell.columns.push_back(newCol);
		}
	}
	
	rows_.push_back(newCell);

	columnHeight_ += newCell.height;

	++numRows_;
}

void TextTable::addColumn(Uint columnWidth, const string& header, CenterType centerType, int color)
{
	ColumnCell newCell;

	newCell.value = "-EMPTY-";			//no value
	newCell.width = columnWidth;
	newCell.color = color;
	newCell.centerType = centerType;

	columnHeaders_.push_back(newCell);
	columnHeaders_.back().value = header;
	columnHeaders_.back().color = color;

	if(rows_.size() == 0)
	{
		addRow();
	}

	RowList::iterator i;
	for(i = rows_.begin(); i != rows_.end(); ++i)
	{
		(*i).columns.push_back(newCell);
	}

	rowWidth_ += newCell.width;

	++numCols_;
}

void TextTable::scrollDown(Uint n)		//TODO: some way to do scrolls without all the casting?
{
	int scrollHeight = max(0, (int)columnHeight_ - (int)displayHeight_);
	int scrollAvailable = max(0, (int)columnHeight_ - (int)scrollHeight - (int)currentY_);	//need greater than 0

	currentY_ += min(scrollAvailable, (int)n);
}

void TextTable::scrollUp(Uint n)
{
	int scrollHeight = max(0, (int)columnHeight_ - (int)displayHeight_);
	int scrollAvailable = (int)currentY_;

	currentY_ -= min(scrollAvailable, (int)n);
}

void TextTable::scrollRight(Uint n)
{
	int scrollWidth = max(0, (int)rowWidth_ - (int)displayWidth_);
	int scrollAvailable = (int)rowWidth_ - (int)scrollWidth - (int)currentX_;
	
	currentY_ += min(scrollAvailable, (int)n);
}

void TextTable::scrollLeft(Uint n)
{
	int scrollWidth = max(0, (int)rowWidth_ - (int)displayWidth_);
	int scrollAvailable = (int)currentX_;
	
	currentY_ -= min(scrollAvailable, (int)n);
}

void TextTable::setBackgroundColor(const Color& c)
{
	backgroundColor_ = c;
}

void TextTable::setPadding(double left, double right, double top, double bottom)
{
	padLeft_ = left;
	padRight_ = right;
	padTop_ = top;
	padBottom_ = bottom;
}
//#include <iostream>

//display
void TextTable::draw() const
{
	double drawWidth = font_.getFontWidth() * displayWidth_;
	double drawHeight = font_.getFontHeight() * displayHeight_;

	//DRAW BACKGROUND
	glDisable(GL_TEXTURE_2D);

	glColor4d(backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a * transparency_);

	glBegin(GL_QUADS);			//background
		glVertex3d(drawWidth + padRight_, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, -drawHeight - padBottom_, 0);
		glVertex3d(drawWidth + padRight_, -drawHeight - padBottom_, 0);
	glEnd();

	glColor4d(1.0, 1.0, 1.0, transparency_);

	glPushMatrix();

	Uint currentHeight=0, currentWidth=0;

	glTranslated(0, -padTop_, 0);	//top padding

	glTranslated(0, -(int)font_.getFontHeight() * 1, 0);	//move to top of table

	//DRAW HEADERS
	glPushMatrix();

	glTranslated(padLeft_, 0, 0);		//translate across

	currentWidth = 0;
	ColumnList::const_iterator h;
	for(h = columnHeaders_.begin(); h != columnHeaders_.end() && currentWidth < displayWidth_; ++h)
	{
		const ColumnCell& col = (*h);

		string dispStr;
		Uint width = col.width;
		if(currentWidth + col.width > displayWidth_)
			width = displayWidth_;

		dispStr = AsciiUtil::fixedWidthString(col.value, width, col.centerType);

		font_.displayString(dispStr.c_str(), col.color);

		glTranslated((double)(col.width * font_.getFontWidth()), 0, 0);		//translate across

		currentWidth += width;
	}
	glPopMatrix();
	glTranslated(0, -(double)(font_.getFontHeight()), 0);			//translate down

	++currentHeight;	//headers count as 1 row

	//printf("DRAWING TABLE\n");

	//DRAW TEXT 
	RowList::const_iterator i = rows_.begin();
	i += currentX_;							//go to 'cursor' position
	for(i; i != rows_.end() && currentHeight < displayHeight_; ++i)
	{
		const RowCell& row = (*i);

		currentWidth = 0;

		glPushMatrix();

		glTranslated(padLeft_, 0, 0);		//left padding

		ColumnList::const_iterator j = row.columns.begin();
		j += currentY_;								//go to 'cursor' position
		for(j; j != row.columns.end() && currentWidth < displayWidth_; ++j)
		{
			const ColumnCell& col = (*j);

			Uint width = col.width;
			if(currentWidth + col.width > displayWidth_)
				width = displayWidth_;

			string dispStr = AsciiUtil::fixedWidthString(col.value, width, col.centerType);

			font_.displayString(dispStr.c_str(), col.color);
			//std::cout << dispStr;

			currentWidth += width;

			glTranslated((double)(col.width * font_.getFontWidth()), 0, 0);		//translate across
		}
		//printf("\n");
		glPopMatrix();	//go back to beginning of row

		currentHeight += row.height;

		glTranslated(0, -(int)(row.height * font_.getFontHeight()), 0);			//translate down
	}

	glPopMatrix();

	//DRAW BORDER
	glDisable(GL_TEXTURE_2D);
	
	//draw border

	glColor4d(0.5, 0.5, 0.5, transparency_);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);			//main border
		glVertex3d(0, 0, 0);
		glVertex3d(drawWidth + padLeft_ + padRight_, 0, 0);
		glVertex3d(drawWidth + padLeft_ + padRight_, -drawHeight - padBottom_ - padTop_, 0);
		glVertex3d(0, -drawHeight - padBottom_ - padTop_, 0);
		glVertex3d(0, 0, 0);
	glEnd();

	glLineWidth(1);
	glBegin(GL_LINES);				//header separator
		glVertex3d(0, -(int)font_.getFontHeight() - padTop_, 0);
		glVertex3d(drawWidth + padRight_, -(int)font_.getFontHeight() - padTop_, 0);
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);
}

