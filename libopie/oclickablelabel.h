#ifndef CLICKABLELABEL
#define CLICKABLELABEL

#include <qlabel.h>

/**
 * This class is a special QLabel which can behave
 * as a QPushButton or QToggleButton.
 * The reason to use a clickable is if you want to save space
 * or you want to skip the border of a normal button
 *
 * <pre>
 *  QLabel* lbl = new OClickableLabel( parent, "PushLabel" );
 *  lbl->setPixmap( "config" );
 *  QWhatsThis::add( lbl, tr("Click here to do something") );
 * </pre>
 *
 * @short A Label behaving as button
 * @author Hakan Ardo, Maximillian Rei� ( harlekin@handhelds.org )
 * @see QLabel
 * @see QPushButton
 * @see QToggleButton
 * @version 1.0
 */

class OClickableLabel: public QLabel
{
  Q_OBJECT
public:
    OClickableLabel(QWidget* parent = 0, const char* name = 0,
		   WFlags fl = 0);
    void setToggleButton(bool t);

 protected:
    void mousePressEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );

 public slots:
    void setOn(bool on);
 signals:
    void clicked();
    void toggled(bool on);
 private:
    bool isToggle : 1;
    bool isDown : 1;
    bool textInverted : 1;

    void showState(bool on);
    void setInverted(bool on);
};

#endif
