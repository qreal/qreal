	class @@NodeName@@ : public ElementImpl
	{
	public:
		void init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}

		void init(QRectF &contents, QList<QPointF> &pointPorts,
							QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
							QList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,
							SdfRendererInterface *portRenderer)
		{
@@NodeLoadShapeRenderer@@
@@NodeLoadPortsRenderer@@
@@NodeSetContents@@
@@NodeInitPointPort@@
@@NodeInitLinePort@@
@@NodeInitLabel@@
		}

		~AcceptEventAction() {}

		void paint(QPainter *painter, QRectF &contents)
		{
			mRenderer->render(painter, contents);
		}

		Qt::PenStyle getPenStyle() { return Qt::SolidLine; }

		void drawStartArrow(QPainter *) const {}
		void drawEndArrow(QPainter *) const {}
		bool hasPorts()
		{
			return @@NodeHasPorts@@;
		}

		void updateData(ElementRepoInterface *repo) const
		{
@@UpdateData@@
		}

		bool isNode()
		{
			return @@IsNode@@;
		}

		bool isContainer()
		{
			return @@IsContainer@@;
		}

		bool isSortContainer()
		{
			return @@IsSortContainer@@;
		}

		int sizeOfForestalling()
		{
			return @@SizeOfForestalling@@;
		}

		int sizeOfChildrenForestalling()
		{
			return @@SizeOfChildrenForestalling@@;
		}

		bool isChildrenMovable()
		{
			return @@IsChildrenMovable@@;
		}

		bool isMinimizingToChildren()
		{
			return @@IsMinimizingToChildren@@;
		}

		bool isClass()
		{
			return @@IsClass@@;
		}

		bool isMaximizingChildren()
		{
			return @@IsMaximizingChildren@@;
		}

		bool isPort()
		{
			return @@IsPort@@;
		}

		bool isHavePin()
		{
			return @@HasPin@@;
		}

		QList<double> getBorders()
		{		
			QList<double> list;
@@NodeGetBorders@@
			return list;
		}

		QStringList bonusContextMenuFields() // not supported yet
		{
			return QStringList();
		}

	private:
		SdfRendererInterface *mRenderer;
@@LabelDefinition@@
	};

