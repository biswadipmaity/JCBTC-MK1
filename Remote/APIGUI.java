import javax.swing.SwingUtilities;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;

import java.awt.Color;
import java.awt.GradientPaint;
import java.awt.RadialGradientPaint;
import java.awt.MultipleGradientPaint;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.Frame;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;

import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;

import java.util.Vector;
import java.util.Date;
import java.util.Calendar;
import java.text.SimpleDateFormat;

public class APIGUI {

	public Vector<Robot> robots = new Vector<Robot>();
	public Vector<String> ports = new Vector<String>();

	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {

				JFrame ui = new JFrame("CONTROL");
				ui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				ui.add(new Control(new APIGUI()));
			 	ui.pack();
				ui.setVisible(true);
				ui.setLocationRelativeTo(null);
				ui.setExtendedState(JFrame.MAXIMIZED_BOTH);

			}
		});
	}

}

class Robot {

	public Port port;
	public Command command;
	public Feed feed;
	private boolean alive;
	private JFrame commandFrame, feedFrame;

	private enum Codes {

		FORWARD ('w'),
		BACKWARD ('s'),
		ANTICLOCKWISE ('a'),
		CLOCKWISE ('d'),
		STOP ('x'),
		UPWARD ('i'),
		DOWNWARD ('k'),
		LEFTWARD ('j'),
		RIGHTWARD ('l');

		public char code;

		private Codes(char value) {
			this.code = value;
		}

	}

	public Robot(String requestPort) {

		System.out.print("\n"+Feedback.timeStamp()+":\tconstructingRobot");

		port = new Port(this);
		alive = port.findPort(requestPort);
		if(!alive)
			return;
		port.openPort();

		command = new Command(this);
		feed = new Feed(this);

		commandFrame = new JFrame("COMMAND");
		commandFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		commandFrame.add(command);
		commandFrame.pack();
		commandFrame.setVisible(true);
		commandFrame.setLocationRelativeTo(null);

		feedFrame = new JFrame("FEED");
		feedFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE); 
		feedFrame.add(feed);
		feedFrame.pack();
		feedFrame.setVisible(true);
		feedFrame.setLocationRelativeTo(null);

		System.out.print("\n"+Feedback.timeStamp()+":\tconstructedRobot");

	}

	public void reset() {

		System.out.print("\n"+Feedback.timeStamp()+":\tdestroyingRobot");

		port.robot = null;

		if(alive) {
			port.closePort();
			port = null;
			command.robot = null;
			command = null;
			feed.robot = null;
			feed = null;
			commandFrame.dispose();
			feedFrame.dispose();
		}

		System.out.print("\n"+Feedback.timeStamp()+":\tdestroyedRobot");

	}

	public void transmit(double r, double theta) {

		System.out.print("\n"+Feedback.timeStamp()+":\ttransmitting");
		
		Codes code;

		if(r<Command.SIZE/8)
			code = Codes.STOP;

		else if(r<Command.SIZE/4)

			if(theta<Math.PI/4 && theta>-Math.PI/4)
				code = Codes.CLOCKWISE;
			else if(theta<Math.PI*3/4 && theta>Math.PI/4)
				code = Codes.FORWARD;
			else if(theta>-Math.PI*3/4 && theta<-Math.PI/4)
				code = Codes.BACKWARD;
			else
				code = Codes.ANTICLOCKWISE;

		else if(r<Command.SIZE*3/8)

			if(theta<Math.PI/4 && theta>-Math.PI/4)
				code = Codes.RIGHTWARD;
			else if(theta<Math.PI*3/4 && theta>Math.PI/4)
				code = Codes.UPWARD;
			else if(theta>-Math.PI*3/4 && theta<-Math.PI/4)
				code = Codes.DOWNWARD;
			else
				code = Codes.LEFTWARD;

		else {
		
			System.out.print("\n"+Feedback.timeStamp()+":\t!transmitted");
			return;

		}

		port.writeData(code.code);

		System.out.print("\n"+Feedback.timeStamp()+":\ttransmitted");

	}

	public boolean isAlive() {
		return alive;
	}

}

class Port {

	public Robot robot;
	private SerialPort serialPort;
	private CommPortIdentifier portId;
	private OutputStream output;
	private static final int TIME_OUT = 2000, DATA_RATE = 9600;

	public Port(Robot bot) {
		robot = bot;
	}

	public boolean findPort(String portName) {

		System.out.print("\n"+Feedback.timeStamp()+":\tfindingPort");

		try {
			portId = CommPortIdentifier.getPortIdentifier(portName);
		}

		catch(Exception e) {
			System.err.println(e.toString());
			System.out.print("\n"+Feedback.timeStamp()+":\t!foundPort");
			return false;
		}
		
		System.out.print("\n"+Feedback.timeStamp()+":\tfoundPort");
		return true;

	}

	public void openPort()   {

		System.out.print("\n"+Feedback.timeStamp()+":\topeningPort");

		try {
			serialPort = (SerialPort) portId.open(this.getClass().getName(),
				TIME_OUT);
			serialPort.setSerialPortParams(DATA_RATE,SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
			output = serialPort.getOutputStream();
		}

		catch (Exception e) {
			System.err.println(e.toString());
			System.out.print("\n"+Feedback.timeStamp()+":\t!openedPort");
		}

		System.out.print("\n"+Feedback.timeStamp()+":\topenedPort");

	}

	public void closePort() {

		System.out.print("\n"+Feedback.timeStamp()+":\tclosingPort");

		try {
			serialPort.removeEventListener();
			serialPort.close();
			serialPort = null;
			output.close();
			output = null;
		}

		catch(Exception e) {
			System.err.println(e.toString());
			System.out.print("\n"+Feedback.timeStamp()+":\t!closedPort");
		}

		System.out.print("\n"+Feedback.timeStamp()+":\tclosedPort");

	}

	public void writeData(char character) {

		System.out.print("\n"+Feedback.timeStamp()+":\twritingData");

		try {
			int a[]=new int[]{0x7E, 0x00, 0x0F, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0xA9, 0xE2, 0xAA, 0xFF, 0xFE, 0x00, 0x00, 0x78, 0x4F};
			byte array[]=new byte[]{(byte)a[0],(byte)a[1],(byte)a[2],(byte)a[3],(byte)a[4],(byte)a[5],(byte)a[6],(byte)a[7],(byte)a[8],(byte)a[9],(byte)a[10],(byte)a[11],(byte)a[12],(byte)a[13],(byte)a[14],(byte)a[15],(byte)a[16],(byte)character,(byte)a[18]};
			int checksum = 0;
			for(int i = 3; i < 18; ++i)
				checksum += array[i];
			array[18] = (byte)(0xFF - (checksum & 0xFF));
			output.write(array);
			output.flush();
		}

		catch (Exception e) {
			System.err.println(e.toString());
			System.out.print("\n"+Feedback.timeStamp()+":\t!writtenData");
		}

		System.out.print("\n"+Feedback.timeStamp()+":\twrittenData");

	}

}

class Command extends JPanel {

	public Robot robot;
	public static final int SIZE = 200;
	private int mouseX, mouseY;
	private boolean mouseClick;

	public Command(Robot bot) {

		robot = bot;
		mouseX = 0;
		mouseY = 0;
		mouseClick = true;

		addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				mouseClick = false;
				repaint();
			}
			public void mouseReleased(MouseEvent e) {
				mouseClick = true;
				repaint();
			}
			public void mouseClicked(MouseEvent e) {
				robot.transmit(Math.hypot(e.getX()-SIZE/2, SIZE/2-e.getY()),
					Math.atan2(SIZE/2-e.getY(), e.getX()-SIZE/2));
			}
		});

		addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseMoved(MouseEvent e) {
				mouseX = e.getX();
				mouseY = e.getY();
				repaint();
			}
		});

	}

	public Dimension getPreferredSize() {
		return new Dimension(SIZE, SIZE);
	}

	protected void paintComponent(Graphics g) {

		Graphics2D g2d = (Graphics2D) g;
		RadialGradientPaint radient;
		GradientPaint gradient;

		int X = (int)(SIZE/2+SIZE/2*Math.cos(Math.atan2
			(mouseY-SIZE/2, mouseX-SIZE/2)));
		int Y = (int)(SIZE/2+SIZE/2*Math.sin(Math.atan2
			(mouseY-SIZE/2, mouseX-SIZE/2)));

		if(mouseClick)
			radient = new RadialGradientPaint (mouseX, mouseY, 100,
				new float[]{0, 1}, new Color[]{Color.WHITE, Color.BLACK});
		else
			radient = new RadialGradientPaint (mouseX, mouseY, 100,
				new float[]{0, 1}, new Color[]{Color.WHITE, Color.DARK_GRAY});
		g2d.setPaint(radient);
		g2d.fillRect(0, 0, SIZE, SIZE);

		gradient = new GradientPaint (X, Y, Color.WHITE,
			SIZE/2, SIZE/2, Color.DARK_GRAY, false);
		g2d.setPaint(gradient);
		g2d.fillOval(SIZE/8, SIZE/8, SIZE*3/4, SIZE*3/4);

		gradient = new GradientPaint (X, Y, Color.DARK_GRAY,
			SIZE/2, SIZE/2, Color.WHITE, false);
		g2d.setPaint(gradient);
		g2d.fillOval(SIZE/4, SIZE/4, SIZE/2, SIZE/2);

		gradient = new GradientPaint (X, Y, Color.WHITE,
			SIZE/2, SIZE/2, Color.DARK_GRAY, false);
		g2d.setPaint(gradient);
		g2d.fillOval(SIZE*3/8, SIZE*3/8, SIZE/4, SIZE/4);

		g2d.setPaint(Color.WHITE);
		g2d.drawOval(SIZE/8, SIZE/8, SIZE*3/4, SIZE*3/4);
		g2d.drawOval(SIZE/4, SIZE/4, SIZE/2, SIZE/2);
		g2d.drawOval(SIZE*3/8, SIZE*3/8, SIZE/4, SIZE/4);

		for(int i=-3; i<4; i+=2)
			g2d.drawLine((int)(SIZE/2+SIZE*3/8*Math.cos(Math.PI*i/4)),
				(int)(SIZE/2+SIZE*3/8*Math.sin(Math.PI*i/4)),
				(int)(SIZE/2+SIZE/8*Math.cos(Math.PI*i/4)),
				(int)(SIZE/2+SIZE/8*Math.sin(Math.PI*i/4)));

	}

}

class Feed extends JPanel {

	public Robot robot;
	public static final int HEIGHT = 360, WIDTH = 480;

	public Feed(Robot bot) {
		robot = bot;
	}

	public Dimension getPreferredSize() {
		return new Dimension(WIDTH, HEIGHT);
	}

	protected void paintComponent(Graphics g) {
		g.setFont(new Font("Arial", Font.BOLD, 14));
		g.setColor(Color.red);
		g.drawString("[INSERT VIDEO HERE]", WIDTH/2-80, HEIGHT/2-10);
	}

}

class Control extends JPanel {

	private APIGUI gui;

	private JTextArea output;
	private JButton more;
	private JButton less;
	private JComboBox<String> list;
	private int mouseX, mouseY;

	public Control(APIGUI ui) {

		gui = ui;
		mouseX = 0;
		mouseY = 0;

		addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseMoved(MouseEvent e) {
				mouseX = e.getX();
				mouseY = e.getY();
				repaint();
			}
		});

		setLayout(new GridBagLayout());
		GridBagConstraints place = new GridBagConstraints();

		output = new JTextArea(4, 25);
		output.setEditable(false);
		System.setOut(new PrintStream(new Feedback(output)));
		place.gridx = 0;
		place.gridy = 1;
		place.gridwidth = 3;
		place.fill = GridBagConstraints.HORIZONTAL;
		place.insets = new Insets(0, 10, 10, 10);
		add(new JScrollPane(output), place);

		more = new JButton("BOOT BOT");
		place.gridx = 1;
		place.gridy = 0;
		place.gridwidth = 1;
		place.insets = new Insets(10, 10, 10, 10);
		add(more, place);

		more.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				String item = (String)list.getSelectedItem();
				if(list.getSelectedIndex()<0) {
					Robot newRobot = new Robot(item);
					if(newRobot.isAlive()) {
						gui.robots.add(newRobot);
						gui.ports.add(item);
					}
				}
			}
		});

		less = new JButton("RESET ROBOT");
		place.gridx = 2;
		add(less, place);

		less.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int index = list.getSelectedIndex();
				if(index<0)
					return;
				gui.robots.elementAt(index).reset();
				gui.robots.remove(index);
				gui.ports.remove(index);
			}
		});

		final DefaultComboBoxModel<String> model = 
			new DefaultComboBoxModel<String>(gui.ports);
		list = new JComboBox<String>(model);
		list.setForeground(Color.BLUE);
		list.setFont(new Font("Arial", Font.BOLD, 14));
		list.setMaximumRowCount(10);
		list.setEditable(true);
		place.gridx = 0;
		add(list, place);

		list.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				JComboBox cb = (JComboBox)event.getSource();
				String item = (String)cb.getSelectedItem();
				if(cb.getSelectedIndex()<0) {
					Robot newRobot = new Robot(item);
					if(newRobot.isAlive()) {
						gui.robots.add(newRobot);
						gui.ports.add(item);
					}
				}
			}
		});

	}

	protected void paintComponent(Graphics g) {

		Graphics2D g2d = (Graphics2D) g;

		GradientPaint gradient = new GradientPaint(0, 0, Color.BLACK,
				getWidth()/2, getHeight()/2, Color.WHITE, true);
		g2d.setPaint(gradient);
		g2d.fillRect(0, 0, getWidth(), getHeight());

		RadialGradientPaint radient = new RadialGradientPaint (mouseX, mouseY,
			1000, new float[]{0, 1}, new Color[]{Color.WHITE, Color.DARK_GRAY});
		g2d.setPaint(radient);
		g2d.fillRect(0, 0, getWidth(), getHeight());

	}

}

class Feedback extends OutputStream {

	private JTextArea output;

	public Feedback(JTextArea area) {
		output = area;
	}

	public void write(int bytes) {

		try {
			output.append(String.valueOf((char)bytes));
		}

		catch (Exception e) {
			System.err.println(e.toString());
		}

	}

	public static String timeStamp() {
		return new SimpleDateFormat("HH:mm:ss").format(Calendar.getInstance().getTime());
	}

}
