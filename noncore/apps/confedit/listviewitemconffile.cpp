/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// (c) 2002 Patrick S. Vogt <tille@handhelds.org>

#include "listviewitemconffile.h"
#include <qmessagebox.h>
#include <qtextstream.h>
#include "listviewitemconfigentry.h"

#define tr QObject::tr

ListViewItemConfFile::ListViewItemConfFile(QFileInfo *file, QListView *parent)
   : ListViewItemConf(parent), _valid(false)
{
  confFileInfo = file;
//  parseFile();
  displayText();
}

ListViewItemConfFile::~ListViewItemConfFile()
{
}


void ListViewItemConfFile::displayText()
{
  setText(0,(_changed?"*":"")+confFileInfo->fileName());	
}

QString ListViewItemConfFile::fileName()
{
	return confFileInfo->fileName();
}

void ListViewItemConfFile::parseFile()
{
	//qDebug("ListViewItemConfFile::parseFile BEGIN");
  QFile confFile(confFileInfo->absFilePath());
	if(! confFile.open(IO_ReadOnly))
 		QMessageBox::critical(0,tr("Could not open"),tr("The file ")+confFileInfo->fileName()+tr(" could not be opened."),1,0);	
	QTextStream t( &confFile );
 	QString s;
  QString group;
  ListViewItemConfigEntry *groupItem;
  ListViewItemConfigEntry *item;
  while ( !t.atEnd() )
  {
  	s = t.readLine().stripWhiteSpace();
		//qDebug( "line: >%s<\n", s.latin1() );
  	if (s.contains("<?xml"))
   	{
    	_valid = false;
			break;
    }else
    if ( s[0] == '[' && s[s.length()-1] == ']' )
   	{
  //    qDebug("got group"+s);
      group = s.mid(1,s.length()-2);
      if (!groupItem) groupItem = new ListViewItemConfigEntry(this, tr("no group") );
      groupItem = new ListViewItemConfigEntry(this, group );
      insertItem( groupItem );
    } else
    if ( int pos = s.find('=') )
    {
//      qDebug("got key"+s);
     	if (!groupItem) qDebug("PANIK NO GROUP! >%s<",group.latin1());
    	item = new ListViewItemConfigEntry(this, group, s );
      groupItem->insertItem( item );
    }
  }	
	confFile.close();
	setExpandable( _valid );
//	qDebug("ListViewItemConfFile::parseFile END");
}


void ListViewItemConfFile::remove()
{
  QFile::remove(confFileInfo->absFilePath());
  QFile::remove(backupFileName());
  delete this;
}

void ListViewItemConfFile::revert()
{
	if (!_changed)
 	{
  	// read the backup file
	 	QFile conf(confFileInfo->absFilePath());
  	QFile back(backupFileName());

	  if (!back.open(IO_ReadOnly)) return;
  	if (!conf.open(IO_WriteOnly)) return;

	  #define SIZE 124
  	char buf[SIZE];
	  while (int c = back.readBlock(buf, SIZE) ) conf.writeBlock(buf,c);
  	conf.close();
	  back.close();
  }
  parseFile();
  expand();
}

void ListViewItemConfFile::save()
{
	if (!_changed) return;
 	QFile conf(confFileInfo->absFilePath());
  QFile back(backupFileName());

  if (!conf.open(IO_ReadOnly)) return;
  if (!back.open(IO_WriteOnly)) return;

  char buf[SIZE];
  while (int c = conf.readBlock(buf, SIZE) ) back.writeBlock(buf,c);
  conf.close();
  back.close();


  if (!conf.open(IO_WriteOnly)) return;
 	QTextStream *t = new QTextStream( &conf );
  for (QListViewItem *it = firstChild(); it!=0;it = it->nextSibling())
  {
   	((ListViewItemConfigEntry*)it)->save(t);
  }
	conf.close();
 	unchanged();
}


bool ListViewItemConfFile::revertable()
{
 	return _changed || QFile(backupFileName()).exists();
}

QString ListViewItemConfFile::backupFileName()
{
	return confFileInfo->absFilePath()+"~";
}


void ListViewItemConfFile::expand()
{
	QListViewItem *subItem = firstChild();
	QListViewItem *toDel;
 	while(subItem)
  {
  	toDel = subItem;
   	subItem = subItem->nextSibling();
   	delete toDel;
  }
	parseFile();
}
