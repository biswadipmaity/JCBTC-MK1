import java.io.*; 
import java.awt.*; 
import java.awt.event.*; 
import java.util.*; 
import javax.swing.*; 

public class Map extends JFrame implements ActionListener { 
	private JPanel row0; 
	private MyJPanel row1; 
	private JButton buttonadd, buttonsubtract, buttonmove; 

	public static void main(String args[]) { 
		new Map(); 
	} 

	public Map() { 
		super("Map"); 
		setResizable(false); 
		setDefaultCloseOperation(EXIT_ON_CLOSE); 
		setLayout(new BorderLayout(1, 1)); 
		row0 = new JPanel(); 
		row0.setLayout(new FlowLayout(FlowLayout.CENTER, 1, 1)); 
		buttonadd = new JButton("Add"); 
		buttonadd.addActionListener(this); 
		row0.add(buttonadd); 
		buttonsubtract = new JButton("Subtract"); 
		buttonsubtract.addActionListener(this); 
		row0.add(buttonsubtract); 
		buttonmove = new JButton("Move"); 
		buttonmove.addActionListener(this); 
		row0.add(buttonmove); 
		add(row0, BorderLayout.PAGE_START); 
		row1 = new MyJPanel(); 
		add(row1, BorderLayout.PAGE_END); 
		pack(); 
		setVisible(true); 
	} 

	public void actionPerformed(ActionEvent ae) { 
		if(ae.getSource() == buttonadd) 
			row1.add(); 
		else if(ae.getSource() == buttonsubtract) 
			row1.subtract(); 
		else if(ae.getSource() == buttonmove) 
			row1.move(); 
	} 

	class MyJPanel extends JPanel implements MouseListener { 
		private LinkedList<Potential> list; 
		private Point lastClicked; 
		private static final int delta = 10; 
		private static final int size = 400; 
		private static final int factor = 400; 

		public MyJPanel() { 
			setPreferredSize(new Dimension(size, size)); 
			list = new LinkedList<Potential>(); 
			lastClicked = null; 
			addMouseListener(this); 
		} 

		public void add() { 
			if(lastClicked != null) 
				list.add(new Potential(lastClicked, factor)); 
			repaint(); 
		} 

		public void subtract() { 
			if(lastClicked != null) 
				list.add(new Potential(lastClicked, -factor)); 
			repaint(); 
		} 

		public void move() { 
			double forceX = 0; 
			double forceY = 0; 
			for(Potential potential : list) 
				if(lastClicked.getX() != potential.point.getX() && lastClicked.getY() != potential.point.getY()) { 
					forceX += potential.weight / Math.pow(((potential.point.getX()-lastClicked.getX())*(potential.point.getX()-lastClicked.getX()) + (potential.point.getY()-lastClicked.getY())*(potential.point.getY()-lastClicked.getY())), 3/2) * (potential.point.getX()-lastClicked.getX()); 
					forceY += potential.weight / Math.pow(((potential.point.getX()-lastClicked.getX())*(potential.point.getX()-lastClicked.getX()) + (potential.point.getY()-lastClicked.getY())*(potential.point.getY()-lastClicked.getY())), 3/2) * (potential.point.getY()-lastClicked.getY()); 
				} 
			lastClicked.setLocation(lastClicked.getX()+forceX, lastClicked.getY()+forceY); 
			repaint(); 
		} 

		public double calculatePotential(double x, double y) {
				double point = 0;
				for(Potential potential : list) 
					if(potential.point.getX() != x && potential.point.getY() != y) 
						point += potential.weight / Math.sqrt ((potential.point.getX()-x)*(potential.point.getX()-x) + (potential.point.getY()-y)*(potential.point.getY()-y)); 
				return point;
		}

		public double[] calculateForce(double x, double y) {
			double force[] = new double[2];
			for(Potential potential : list) 
				if(lastClicked.getX() != potential.point.getX() && lastClicked.getY() != potential.point.getY()) { 
					force[0] += potential.weight / Math.pow(((potential.point.getX()-x)*(potential.point.getX()-x) + (potential.point.getY()-y)*(potential.point.getY()-y)), 3/2) * (potential.point.getX()-x); 
					force[1] += potential.weight / Math.pow(((potential.point.getX()-x)*(potential.point.getX()-x) + (potential.point.getY()-y)*(potential.point.getY()-y)), 3/2) * (potential.point.getY()-y); 
				}
			return force;
		}

		public void mousePressed(MouseEvent e) { 
		} 

		public void mouseReleased(MouseEvent e) { 
		} 

		public void mouseEntered(MouseEvent e) { 
		} 

		public void mouseExited(MouseEvent e) { 
		} 

		public void mouseClicked(MouseEvent e) { 
			lastClicked = e.getPoint(); 
			repaint(); 
		} 

		public void paint(Graphics g) { 
			Graphics2D g2 = (Graphics2D) g.create(); 
			g2.setColor(Color.WHITE); 
			g2.fillRect(0, 0, size, size); 
			int i, j; 
			for(i = 0; i < size; ++i) { 
				for(j = 0; j < size; ++j) { 
					double point = calculatePotential(i, j); 
					if(point < -95 && point > -105) 
						g2.setColor(Color.GRAY); 
					else if(point < -45 && point > -55) 
						g2.setColor(Color.BLUE); 
					else if(point < 5 && point > -5) 
						g2.setColor(Color.GREEN); 
					else if(point < 55 && point > 45) 
						g2.setColor(Color.YELLOW); 
					else if(point < 105 && point > 95) 
						g2.setColor(Color.ORANGE); 
					else 
						g2.setColor(Color.WHITE); 
					g2.fillRect(i, j, 20, 20); 
				} 
			} 
			g2.setColor(Color.BLACK); 
			for (i = 0; i < list.size(); ++i) 
				g2.drawOval((int)list.get(i).point.getX()-5, (int)list.get(i).point.getY()-5, 10, 10); 
			if(lastClicked != null) { 
				g2.setColor(Color.BLACK); 
				double force[] = calculateForce(lastClicked.getX(), lastClicked.getY()); 
				g2.drawLine((int)lastClicked.getX(), (int)lastClicked.getY(), (int)lastClicked.getX()+(int)force[0], (int)lastClicked.getY()+(int)force[1]); 
				g2.setColor(Color.BLUE); 
				g2.drawOval((int)lastClicked.getX()-5, (int)lastClicked.getY()-5, 10, 10); 
			} 
			g2.dispose(); 
		} 
	} 

	class Potential { 
		public Point point; 
		public double weight; 
		public Potential(Point point, double weight) { 
			this.point = point; 
			this.weight = weight; 
		} 
	} 
}
