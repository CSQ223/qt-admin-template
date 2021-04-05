#include <QPainter>
#include <QDebug>
#include <QVariant>
#include <QMetaType>
#include <QFontDatabase>
#include <QStandardItemModel>

#include "itemdef.h"
#include "customlistview.h"

static QChar strtohex(QString str)
{
    unsigned short data = 0;

    if(str.isEmpty())
    {
        data = 0;
    }else{
        for(int i=0; i<str.size(); i++)
        {
            char ch = str.at(i).toLatin1();
            if((ch >= '0') && (ch <= '9'))
            {
                data = data*16 + (ch - '0');
            }

            if((ch >= 'a') &&(ch <= 'f'))
            {
                data = data*16 + ch - 'a' + 10;
            }

            if((ch >= 'A') &&(ch <= 'F'))
            {
                data = data*16 + ch - 'A' + 10;
            }
        }
    }

    return QChar(data);
}

NavDelegate::NavDelegate(QObject *parent, CustomListView *nav_ptr)
{
//    this->setEditorData(parent, )
    nav = nav_ptr;

    //qDebug() << "nav->getParentHeight(): "<< nav->getParentHeight();

    //判断图形字体是否存在,不存在则加入
    QFontDatabase fontDb;
    if (!fontDb.families().contains("FontAwesome")) {
        int fontId = fontDb.addApplicationFont(":/resources/images/fontawesome-webfont.ttf");
        QStringList fontName = fontDb.applicationFontFamilies(fontId);
        if (fontName.count() == 0) {
            qDebug() << "load fontawesome-webfont.ttf error";
        }

    }

    if (fontDb.families().contains("FontAwesome")) {
        iconFont = QFont("FontAwesome");
#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
        iconFont.setHintingPreference(QFont::PreferNoHinting);
#endif
    }
}

NavDelegate::~NavDelegate()
{

}

QSize NavDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(80, 30);
}

void NavDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    NavModel::TreeNode *node = (NavModel::TreeNode *)index.data(Qt::UserRole).toULongLong();
#if 1
    //定义变量存储区域
    QRect optionRect = option.rect;
    int x = optionRect.x();
    int y = optionRect.y();
    int width = optionRect.width();
    int height = optionRect.height();

    int fontSize = nav->getParentFontSize();
    qDebug() << node->text << " "<< node->icon ;
    //父节点和子节点颜色分开设置
    bool parent = (node->level == 1);

    //根据不同的状态设置不同的颜色 bgColor-主背景色 textColor-主文字颜色 tipBgColor-提示信息背景颜色 tipTextColor-提示信息文字颜色
    QColor bgColor, textColor, tipBgColor, tipTextColor, iconColor;
    if (option.state & QStyle::State_Selected) {
        bgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        textColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
    } else if (option.state & QStyle::State_MouseOver) {
        bgColor = parent ? nav->getParentBgHoverColor() : nav->getChildBgHoverColor();
        textColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
    } else {
        bgColor = parent ? nav->getParentBgNormalColor() : nav->getChildBgNormalColor();
        textColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
        tipBgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        tipTextColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        iconColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
    }

    //绘制背景
    painter->fillRect(optionRect, bgColor);

    //绘制线条,目前限定子节点绘制,如果需要父节点也绘制则取消parent判断即可
    int lineWidth = nav->getLineWidth();
    if (!parent && nav->getLineVisible() && lineWidth > 0) {
        if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver)) {
            //设置偏移量,不然上下部分会有点偏移
            float offset = (float)lineWidth / 2;

            //计算上下两个坐标点
            QPointF pointTop(x, y + offset);
            QPointF pointBottom(x, height + y - offset);
            if (!nav->getLineLeft()) {
                pointTop.setX(width);
                pointBottom.setX(width);
            }

            //设置线条颜色和宽度
            painter->setPen(QPen(nav->getLineColor(), lineWidth));
            painter->drawLine(pointTop, pointBottom);
        }
    }

    //绘制三角形,目前限定子节点绘制,如果需要父节点也绘制则取消parent判断即可
    int triangleWidth = nav->getTriangleWidth();
    if (!parent && nav->getTriangleVisible() && triangleWidth > 0) {
        if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver)) {
            QFont font = iconFont;
            font.setPixelSize(fontSize + triangleWidth);
            painter->setFont(font);
            painter->setPen(nav->getTriangleColor());

            //采用图形字体中的三角形绘制
            if (nav->getTriangleLeft()) {
                painter->drawText(optionRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf0da));
            } else {
                painter->drawText(optionRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf0d9));
            }
        }
    }

    //绘制行分隔符
    if (nav->getSeparateVisible()) {
        if (node->level == 1 || (node->level == 2 && node->last)) {
            painter->setPen(QPen(nav->getSeparateColor(), nav->getSeparateHeight()));
            painter->drawLine(QPointF(x, y + height), QPointF(x + width, y + height));
        }
    }

    //绘制文字,如果文字为空则不绘制
    QString text = node->text;
    //qDebug() << text;
    if (!text.isEmpty()) {
        //文字离左边的距离+字体大小
        int margin = nav->getParentMargin();
        if (node->level == 2) {
            margin = nav->getChildMargin();
            fontSize = nav->getChildFontSize();
        }

        //计算文字区域
        QRect textRect = optionRect;
        textRect.setWidth(width - margin);
        textRect.setX(x + margin);

        QFont font;
        font.setPixelSize(fontSize);
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    //绘制提示信息,如果不需要显示提示信息或者提示信息为空则不绘制
    QString tip = node->tip;
    if (nav->getTipVisible() && !tip.isEmpty()) {
        //如果是数字则将超过999的数字显示成 999+
        //如果显示的提示信息长度过长则将多余显示成省略号 .
        if (tip.toInt() > 0) {
            tip = tip.toInt() > 999 ? "999+" : tip;
        } else if (tip.length() > 2) {
            tip = tip.left(2) + " .";
        }

        //计算绘制区域,半径取高度的四分之一
        int radius = height / 4;
        QRect tipRect = optionRect;
        tipRect.setHeight(radius * 2);
        tipRect.moveCenter(optionRect.center());
        tipRect.setLeft(optionRect.right() - nav->getTipWidth() - 5);
        tipRect.setRight(optionRect.right() - 5);

        //设置字体大小
        QFont font;
        font.setPixelSize(fontSize - 2);
        painter->setFont(font);

        //绘制提示文字的背景
        painter->setPen(Qt::NoPen);
        painter->setBrush(tipBgColor);
        painter->drawRoundedRect(tipRect, radius, radius);

        //绘制提示文字
        painter->setPen(tipTextColor);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(tipRect, Qt::AlignCenter, tip);
    }

    //计算绘制图标区域
    QRect iconRect = optionRect;
    iconRect.setLeft(parent ? nav->getParentIconMargin() : nav->getChildIconMargin());

    //设置图形字体和画笔颜色
    QFont font = iconFont;
    font.setPixelSize(fontSize);
    painter->setFont(font);
    painter->setPen(textColor);

    //绘制左侧图标,有图标则绘制图标,没有的话父窗体取 + -
    if (!node->icon.isNull()) {
        painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, node->icon);
    } else if (parent) {
        if (node->expand) {
            painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf068));
        } else {
            painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf067));
        }
    }

    //绘制父节点右侧图标
    iconRect.setRight(optionRect.width() - 10);
    if (!(nav->getTipVisible() && !node->tip.isEmpty()) && nav->getRightIconVisible() && parent) {
        if (node->expand) {
            painter->drawText(iconRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf078));
        } else {
            painter->drawText(iconRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf054));
        }
    }
#endif
}

NavModel::NavModel(QObject *parent)
{

}

NavModel::~NavModel()
{

}

int NavModel::rowCount(const QModelIndex &parent) const
{
    return listNode.size();
}

QVariant NavModel::data(const QModelIndex &index, int role) const
{
    qulonglong node_ptr_addr;
    //if (!index.isValid())
    //    return QVariant();

    //if (index.row() >= listNode.count())
    //    return QVariant();
//    qDebug() << "role:" << role << "index.row:" << index.row() << " column:" <<index.column() ;
    if(role == Qt::UserRole)
    {
//        const NavModel::TreeNode *node_ptr = treeNode.at(index.row());
        const NavModel::TreeNode *node_ptr = listNode.at(index.row()).treeNode;
//        if(index.row() == 1)
//        {
//            node_ptr->level = 2;
//        }
        node_ptr_addr = (qulonglong)node_ptr;
        //qDebug() << "node_ptr->text:" << node_ptr->text;
        //
        //ListNode node = listNode.at(index.row());
        //qMetatypeId<NavModel::ListNode>();
        //return QVariant(treeNode[0]);
        //return QVariant(treeNode[0]->text);

    } /*else {
        const NavModel::TreeNode *node_ptr = treeNode.at(index.row())->children.at(0);
        node_ptr_addr = (qulonglong)node_ptr;
        //qDebug() << "node_ptr->text:" << node_ptr->text;
        //qDebug() << "else role:" << role << "index.row:" << index.row() << " column:" <<index.column() ;
    }*/
    return QVariant(node_ptr_addr);
}

void NavModel::setItems(const QStringList &items)
{
    int level = 0;
    NavModel::ListNode node;
    NavModel::TreeNode *node_ptr= new NavModel::TreeNode;

    node.text = items[4];
    level = items[0].toInt();

    if(level == 1)
    {
        node_ptr->level = 1;
        node_ptr->expand = (items[1].toInt() ? true : false);
        node_ptr->last = (items[2].toInt() ? true : false);
        if(items[3].isEmpty())
            node_ptr->icon = 0;
        else
            node_ptr->icon = 0xf067;
        node_ptr->icon = strtohex(items[3]);
        node_ptr->text = items[4];
        if(items[5].isEmpty())
            node_ptr->tip = "";
        node_ptr->parentText = items[6];

        node.treeNode = node_ptr;

        treeNode.append(node_ptr);

        listNode.append(node);
    }else if(level == 2)
    {
        node_ptr->level = 2;
        node_ptr->expand = (items[1].toInt() ? true : false);
        node_ptr->last = (items[2].toInt() ? true : false);
        node_ptr->icon = 0xf067;
        node_ptr->icon = strtohex(items[3]);
        node_ptr->text = items[4];
        node_ptr->tip = items[5];
        node_ptr->parentText = items[6];

        foreach (TreeNode *parent_node_ptr, treeNode)
        {
            if(parent_node_ptr->text == node_ptr->parentText)
            {

                parent_node_ptr->children.append(node_ptr);

                if(parent_node_ptr->expand)
                {
                    node.treeNode = node_ptr;
                    listNode.append(node);
                }

            }
        }
    }
    refreshList();
}

void NavModel::expand(const QModelIndex &index)
{
    TreeNode *temp_ptr = listNode[index.row()].treeNode;

    temp_ptr->expand = temp_ptr->expand ? false : true;

    refreshList();
}

void NavModel::refreshList()
{
    listNode.clear();

    for(int i=0; i<treeNode.count(); i++)
    {
        ListNode node;
        node.text = "test";
        node.treeNode = treeNode[i];
        listNode.push_back(node);

        if(treeNode[i]->expand)
        {
            for(int j=0;j<treeNode[i]->children.count();j++)
            {
                ListNode node1;
                node1.text = "test";
                node1.treeNode = treeNode[i]->children[j];
                listNode.push_back(node1);
            }
        }

    }

    beginInsertRows(QModelIndex(),listNode.count(),listNode.count());
    endInsertRows();
}

CustomListView::CustomListView(QWidget *parent) : QListView (parent)
{
    //items;                                //节点集合
    rightIconVisible = true;                //右侧图标是否显示
    tipVisible = true;                      //是否显示提示信息
    tipWidth = 10;                          //提示信息宽度

    separateVisible = true;                 //是否显示行分隔符
    separateHeight = 5;                     //行分隔符高度
    separateColor = QColor(255, 255, 255);  //行分隔符颜色

    lineLeft = true;                        //是否显示在左侧
    lineVisible = true;                     //是否显示线条
    lineWidth = 20;                         //线条宽度
    lineColor = QColor(255, 255, 255);      //线条颜色

    triangleLeft = true;                    //是否显示在左侧
    triangleVisible = true;                 //是否显示三角形
    triangleWidth = 10;                     //三角形宽度
    triangleColor = QColor(255, 255, 255);  //三角形颜色

    parentIconMargin =5;                   //父节点图标边距
    parentMargin = 20;                      //父节点边距
    parentFontSize = 14;                    //父节点字体大小
    parentHeight = 20;                      //父节点高度

    parentBgNormalColor = QColor(52, 72, 93);        //父节点正常背景色
    parentBgSelectedColor = QColor(24, 189, 155);    //父节点选中背景色
    parentBgHoverColor = QColor(125, 125, 125);       //父节点悬停背景色
    parentTextNormalColor = QColor(25, 200, 205);         //父节点正常文字颜色
    parentTextSelectedColor = QColor(255, 255, 255);        //父节点选中文字颜色
    parentTextHoverColor = QColor(255, 255, 255);       //父节点悬停文字颜色

    childIconMargin = 20;                               //子节点图标边距
    childMargin = 40;                                   //子节点边距
    childFontSize = 12;                                 //子节点字体大小
    childHeight = 16;                                   //子节点高度
    childBgNormalColor = QColor(52, 72, 93);            //子节点正常背景色
    childBgSelectedColor = QColor(24, 189, 155);       //子节点选中背景色
    childBgHoverColor = QColor(25, 200, 205);          //子节点悬停背景色
    childTextNormalColor = QColor(255, 255, 255);       //子节点正常文字颜色
    childTextSelectedColor = QColor(255, 255, 255);     //子节点选中文字颜色
    childTextHoverColor = QColor(255, 255, 255);        //子节点悬停文字颜色

    //childTextNormalColor = QColor(255, 255, 255);       //子节点悬停文字颜色

    expendMode = ExpendMode_SingleClick;                //节点展开模式 单击/双击/禁用





    delegate = new NavDelegate(parent, this);

    model = new NavModel(this);
//    model->setItems(parentItem);
    //delegate->setEditorData() //(parent, );

    int totalNum = 3;
    int redNum = 0;
    int blueNum = 0;
    int yellowNum = 0;
    QStringList strlist;
//    m_model = new QStandardItemModel();

    for (int i = 0; i < parentItem.count(); ++i) {
        strlist.clear();
        strlist.append(parentItem[i]);
        for(int j=0; j<childItem[i].count(); j++)
        {
            strlist.append(childItem[i].at(j));
        }

        model->setItems(strlist);
    }

    this->setModel(model);
    this->setItemDelegate(delegate);

    if(expendMode == ExpendMode_SingleClick)
    {
        connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(pressed(const QModelIndex &)));
    } else if(expendMode == ExpendMode_DoubleClick)
    {
        connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(pressed(const QModelIndex &)));
    } else {

    }
    //
    //doubleClicked(const QModelIndex &index)
    //clicked(const QModelIndex &index)
}

CustomListView::~CustomListView()
{
    delete delegate;

    delete model;
}

void CustomListView::pressed(const QModelIndex &data)
{
//    qDebug() << "pressed data.row():"<< data.row()<< " data.coloum():" << data.column();
//    delegate->setEditorData(this, data);
    model->expand(data);
    int row = data.row();
    int coloum = data.column();
    emit pressed(data.row());
}

void CustomListView::setData(const QStringList &listItems)
{
    //model->setItems(listItems);
}

QString CustomListView::getItems() const
{
    return "";
}

bool CustomListView::getRightIconVisible() const
{
    return rightIconVisible;
}

bool CustomListView::getTipVisible() const
{
    return tipVisible;
}

int CustomListView::getTipWidth() const
{
    return tipWidth;
}

bool CustomListView::getSeparateVisible() const
{
    return tipVisible;
}

int CustomListView::getSeparateHeight() const
{
    return separateHeight;
}

QColor CustomListView::getSeparateColor() const
{
    return separateColor;
}

bool CustomListView::getLineLeft() const
{
    return 0;
}

bool CustomListView::getLineVisible() const
{
    return lineVisible;
}

int CustomListView::getLineWidth() const
{
    return lineWidth;
}

QColor CustomListView::getLineColor() const
{
    return lineColor;
}

bool CustomListView::getTriangleLeft() const
{
    return triangleLeft;
}

bool CustomListView::getTriangleVisible() const
{
    return triangleVisible;
}

int CustomListView::getTriangleWidth() const
{
    return triangleWidth;
}

QColor CustomListView::getTriangleColor() const
{
    return triangleColor;
}

int CustomListView::getParentIconMargin() const
{
    return parentIconMargin;
}

int CustomListView::getParentMargin() const
{
    return parentMargin;
}

int CustomListView::getParentFontSize() const
{
    //qDebug() << "return parentFontSize:" << parentFontSize ;
    return parentFontSize;
}

int CustomListView::getParentHeight() const
{
    return parentHeight;
}

QColor CustomListView::getParentBgNormalColor() const
{
    return parentBgNormalColor;
}

QColor CustomListView::getParentBgSelectedColor() const
{
    return parentBgSelectedColor;
}

QColor CustomListView::getParentBgHoverColor() const
{
    return parentBgHoverColor;
}

QColor CustomListView::getParentTextNormalColor() const
{
    return parentTextHoverColor;
}

QColor CustomListView::getParentTextSelectedColor() const
{
    return parentTextSelectedColor;
}

QColor CustomListView::getParentTextHoverColor() const
{
    return parentTextHoverColor;
}

int CustomListView::getChildIconMargin() const
{
    return  childIconMargin;
}

int CustomListView::getChildMargin() const
{
    return childMargin;
}

int CustomListView::getChildFontSize() const
{
    return childFontSize;
}

int CustomListView::getChildHeight() const
{
    return childHeight;
}

QColor CustomListView::getChildBgNormalColor()  const
{
    return childBgNormalColor;
}

QColor CustomListView::getChildBgSelectedColor()const
{
    return childBgSelectedColor;
}

QColor CustomListView::getChildBgHoverColor()   const
{
    return childBgHoverColor;
}

QColor CustomListView::getChildTextNormalColor()const
{
    return childTextNormalColor;
}

QColor CustomListView::getChildTextSelectedColor()const
{
    return childTextSelectedColor;
}

QColor CustomListView::getChildTextHoverColor() const
{
    return childTextHoverColor;
}

ExpendMode CustomListView::getExpendMode() const
{
    return expendMode;
}

QSize CustomListView::sizeHint() const
{
    return QSize(60, 60);
}

QSize CustomListView::minimumSizeHint() const
{
    return QSize(0, 0);
}

//设置节点数据
void CustomListView::setItems(const QString &items)
{
    //parent:1,0,0/1,0x67,主界面,父节点,NULL
    //children:2,0,0/1,0x67,子节点,子节点,主界面
    //系统设置

    this->items = items;

//    QStringList strlist = items.split(",", QString::SkipEmptyParts);
    QStringList strlist = items.split(",");

    int level = strlist[0].toInt();

    switch (level)
    {
    case 1:
    {
        if(!strlist[4].isEmpty())
            parentItem.append(strlist[4]);
        break;
    }
    case 2:
    {
        int index = 0;

        if(!strlist[6].isEmpty())
        {
            break;
        }

        foreach (QString str, parentItem)
        {
            if(str == strlist[6])
            {
                break;
            }
            index++;
        }

        childItem[index].append(strlist[6]);

        break;
    }
    default:

        break;
    }

    model->setItems(strlist);

}

//设置选中指定行
void CustomListView::setCurrentRow(int row)
{

}

//设置父节点右侧图标是否显示
void CustomListView::setRightIconVisible(bool rightIconVisible)
{

}

//设置提示信息 是否显示+宽度
void CustomListView::setTipVisible(bool tipVisible)
{

}

void CustomListView::setTipWidth(int tipWidth)
{

}
//设置行分隔符 是否显示+高度+颜色
void CustomListView::setSeparateVisible(bool separateVisible)
{

}

void CustomListView::setSeparateHeight(int separateHeight)
{

}

void CustomListView::setSeparateColor(const QColor &separateColor)
{

}

//设置线条 位置+可见+宽度+颜色
void CustomListView::setLineLeft(bool lineLeft)
{

}

void CustomListView::setLineVisible(bool lineVisible)
{

}

void CustomListView::setLineWidth(int lineWidth)
{

}

void CustomListView::setLineColor(const QColor &lineColor)
{

}

//设置三角形 位置+可见+宽度+颜色
void CustomListView::setTriangleLeft(bool triangleLeft)
{

}

void CustomListView::setTriangleVisible(bool triangleVisible)
{

}

void CustomListView::setTriangleWidth(int triangleWidth)
{

}

void CustomListView::setTriangleColor(const QColor &triangleColor)
{

}

//设置父节点 图标边距+左侧边距+字体大小+节点高度+颜色集合
void CustomListView::setParentIconMargin(int parentIconMargin)
{

}

void CustomListView::setParentMargin(int parentMargin)
{

}

void CustomListView::setParentFontSize(int parentFontSize)
{

}

void CustomListView::setParentHeight(int parentHeight)
{

}

void CustomListView::setParentBgNormalColor(const QColor &parentBgNormalColor)
{

}

void CustomListView::setParentBgSelectedColor(const QColor &parentBgSelectedColor)
{

}

void CustomListView::setParentBgHoverColor(const QColor &parentBgHoverColor)
{

}

void CustomListView::setParentTextNormalColor(const QColor &parentTextNormalColor)
{

}

void CustomListView::setParentTextSelectedColor(const QColor &parentTextSelectedColor)
{

}

void CustomListView::setParentTextHoverColor(const QColor &parentTextHoverColor)
{

}

//设置子节点 图标边距+左侧边距+字体大小+节点高度+颜色集合
void CustomListView::setChildIconMargin(int childIconMargin)
{

}

void CustomListView::setChildMargin(int childMargin)
{

}

void CustomListView::setChildFontSize(int childFontSize)
{

}

void CustomListView::setChildHeight(int childHeight)
{

}

void CustomListView::setChildBgNormalColor(const QColor &childBgNormalColor)
{

}

void CustomListView::setChildBgSelectedColor(const QColor &childBgSelectedColor)
{

}

void CustomListView::setChildBgHoverColor(const QColor &childBgHoverColor)
{

}

void CustomListView::setChildTextNormalColor(const QColor &childTextNormalColor)
{

}

void CustomListView::setChildTextSelectedColor(const QColor &childTextSelectedColor)
{

}

void CustomListView::setChildTextHoverColor(const QColor &childTextHoverColor)
{

}

//设置节点展开模式
void CustomListView::setExpendMode(const ExpendMode &expendMode)
{

}
//Q_SIGNALS:
//void pressed(const QString &text, const QString &parentText);
//void pressed(int index, int parentIndex);
//void pressed(int childIndex);
