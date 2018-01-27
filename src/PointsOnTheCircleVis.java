import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.util.*;
import java.security.*;
import javax.swing.*;
import javax.imageio.*;

// --------------------------------------------------------
public class PointsOnTheCircleVis {
    static int maxN = 200, minN = 20;
    static int maxP = 200, minP = 5;
    int N; // number of points
    int[][] matrix; // input matrix
    int[] perm; // permutation of the points
    // ---------------------------------------------------

    String generate(String seedStr) {
        try {
            SecureRandom rnd = SecureRandom.getInstance("SHA1PRNG");
            long seed = Long.parseLong(seedStr);
            rnd.setSeed(seed);

            N = rnd.nextInt(maxN - minN + 1) + minN;
            int edgeProb = rnd.nextInt(maxP - minP + 1) + minP;
            if (seed == 1) {
                N = 8;
                edgeProb = 200;
            } else if (seed == 2)
                N = minN;
            else if (seed == 3)
                N = maxN;

            // generate the input matrix (connectivity between points)
            // a random symmetric matrix with 0s on diagonal
            // 
            int nEdge = 0;
            matrix = new int[N][N];
            for (int i = 0; i < N; ++i)
                for (int j = i + 1; j < N; ++j) {
                    matrix[i][j] = matrix[j][i] = (rnd.nextInt(1000) < edgeProb ? 1 : 0);
                    if (matrix[i][j] == 1)
                        nEdge++;
                }

            StringBuffer sb = new StringBuffer();
            sb.append("N = ").append(N).append('\n');
            sb.append("Edge probability = ").append(edgeProb * 0.1).append("%\n");
            sb.append("Number of edges = ").append(nEdge).append('\n');
            if (seed < 3)
                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j)
                        sb.append(matrix[i][j]).append(' ');
                    sb.append('\n');
                }
            return sb.toString();
        } catch (Exception e) {
            addFatalError("An exception occurred while generating test case.");
            e.printStackTrace();
            return "";
        }
    }

    // ---------------------------------------------------
    public double runTest(String seed) {
        try {
            String test = generate(seed);
            if (debug)
                System.out.println(test);

            // call user's solution and get return
            int[] mArg = new int[N * N];
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    mArg[i * N + j] = matrix[i][j];

            if (proc != null) {
                try {
                    perm = permute(mArg);
                } catch (Exception e) {
                    addFatalError("Failed to get result from permute.");
                    return -1;
                }

                if (perm.length != N) {
                    addFatalError("Your return must contain " + N + " elements, and it contained " + perm.length + ".");
                    return -1;
                }

                // check that this is a valid permutation
                boolean[] used = new boolean[N];
                for (int i = 0; i < N; ++i) {
                    if (perm[i] < 0 || perm[i] >= N) {
                        addFatalError("All elements of your return must be between 0 and " + (N - 1)
                                + ", and your return contained " + perm[i] + ".");
                        return -1;
                    }
                    if (used[perm[i]]) {
                        addFatalError("All elements of your return must be unique, and your return contained " + perm[i]
                                + " twice.");
                        return -1;
                    }
                    used[perm[i]] = true;
                }
            } else {
                // use the identity permutation (for visualization/scoring demo)
                perm = new int[N];
                for (int i = 0; i < N; ++i)
                    perm[i] = i;
            }

            if (debug) {
                System.err.println("Permutation:");
                for (int i = 0; i < N; ++i)
                    System.err.print(perm[i] + " ");
                System.err.println();
            }

            // calculate score
            double score = 0;
            // score is sum of lengths of lines connecting permuted points (assuming circle radius = 1)
            for (int i = 0; i < N; ++i)
                for (int j = i + 1; j < N; ++j)
                    if (matrix[perm[i]][perm[j]] == 1) {
                        double alpha = Math.PI / N * (j - i);
                        score += 2 * Math.sin(alpha);
                        if (debug)
                            System.err.println("p[" + i + "] = " + perm[i] + ", " + "p[" + j + "] = " + perm[j] + ", "
                                    + "angle = " + alpha + ", " + "length = " + (2 * Math.sin(alpha)));
                    }

            if (vis) {
                // draw the image
                jf.setSize(SZX + 10, SZY + 40);
                jf.setVisible(true);
                draw();
            }

            return score;
        } catch (Exception e) {
            addFatalError("An exception occurred while trying to process your program's results.");
            e.printStackTrace();
            return -1;
        }
    }

    // ------------- visualization part ----------------------
    static String exec, fileName;
    static boolean vis, debug, save;
    static Process proc;
    JFrame jf;
    Vis v;
    InputStream is;
    OutputStream os;
    BufferedReader br;
    // problem-specific drawing params
    static int SZX, SZY, SZ;
    static final int D = 64;

    // ---------------------------------------------------
    int[] permute(int[] m) throws IOException {
        // pass the params to the solution and get the return
        int i;
        StringBuffer sb = new StringBuffer();
        sb.append(m.length).append('\n');
        for (i = 0; i < m.length; ++i)
            sb.append(m[i]).append('\n');
        os.write(sb.toString().getBytes());
        os.flush();

        // get the return - an array of integers
        int nret = Integer.parseInt(br.readLine());
        int[] ret = new int[nret];
        for (i = 0; i < nret; ++i)
            ret[i] = Integer.parseInt(br.readLine());
        return ret;
    }

    // ---------------------------------------------------
    void draw() {
        if (!vis)
            return;
        v.repaint();
    }

    // ---------------------------------------------------
    public class Vis extends JPanel implements WindowListener {
        public void paint(Graphics g) {
            try {
                // do painting here
                BufferedImage bi = new BufferedImage(SZX, SZY, BufferedImage.TYPE_INT_RGB);
                Graphics2D g2 = (Graphics2D) bi.getGraphics();
                // background
                g2.setColor(new Color(0xD3D3D3));
                g2.fillRect(0, 0, SZX, SZY);

                int n = 2;
                // draw the circle
                g2.setColor(Color.WHITE);
                g2.fillOval(D, D, 2 * SZ, 2 * SZ);
                g2.setColor(Color.BLACK);

                // draw the lines between points
                for (int i = 0; i < N; ++i)
                    for (int j = i + 1; j < N; ++j)
                        if (matrix[perm[i]][perm[j]] == 1)
                            g2.drawLine(SZ + (int) Math.round(SZ * Math.sin(2 * i * Math.PI / N)) + D,
                                    SZ - (int) Math.round(SZ * Math.cos(2 * i * Math.PI / N)) + D,
                                    SZ + (int) Math.round(SZ * Math.sin(2 * j * Math.PI / N)) + D,
                                    SZ - (int) Math.round(SZ * Math.cos(2 * j * Math.PI / N)) + D);

                // draw the points on the circle
                for (int i = 0; i < N; ++i) {
                    g2.drawOval(SZ + (int) Math.round(SZ * Math.sin(2 * i * Math.PI / N)) + D - 2,
                            SZ - (int) Math.round(SZ * Math.cos(2 * i * Math.PI / N)) + D - 2, 4, 4);
                }

                // add labels with points' indices
                g2.setFont(new Font("Arial", Font.BOLD, 22));
                FontMetrics fm = g2.getFontMetrics();
                char[] ch;
                for (int i = 0; i < N; ++i) {
                    ch = ("" + perm[i]).toCharArray();
                    int x = SZ + D + (int) Math.round((SZ + D / 2) * Math.sin(2 * i * Math.PI / N)) - 5;
                    int y = SZ + D - (int) Math.round((SZ + D / 2) * Math.cos(2 * i * Math.PI / N)) + fm.getHeight() / 2
                            - 2;
                    g2.drawChars(ch, 0, ch.length, x, y);
                }

                g.drawImage(bi, 0, 0, SZX, SZY, null);
                if (save) {
                    ImageIO.write(bi, "png", new File(fileName + ".png"));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        public Vis() {
            jf.addWindowListener(this);
        }

        // WindowListener
        public void windowClosing(WindowEvent e) {
            if (proc != null)
                try {
                    proc.destroy();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            System.exit(0);
        }

        public void windowActivated(WindowEvent e) {
        }

        public void windowDeactivated(WindowEvent e) {
        }

        public void windowOpened(WindowEvent e) {
        }

        public void windowClosed(WindowEvent e) {
        }

        public void windowIconified(WindowEvent e) {
        }

        public void windowDeiconified(WindowEvent e) {
        }
    }

    // ---------------------------------------------------
    public PointsOnTheCircleVis(String seed) {
        //interface for runTest
        if (vis) {
            jf = new JFrame();
            v = new Vis();
            jf.getContentPane().add(v);
        }
        if (exec != null) {
            try {
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec(exec);
                os = proc.getOutputStream();
                is = proc.getInputStream();
                br = new BufferedReader(new InputStreamReader(is));
                new ErrorReader(proc.getErrorStream()).start();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println(runTest(seed));
        if (proc != null)
            try {
                proc.destroy();
            } catch (Exception e) {
                e.printStackTrace();
            }
    }

    // ---------------------------------------------------
    public static void main(String[] args) {
        String seed = "1";
        SZ = 900;
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-seed"))
                seed = args[++i];
            if (args[i].equals("-exec"))
                exec = args[++i];
            if (args[i].equals("-vis"))
                vis = true;
            if (args[i].equals("-debug"))
                debug = true;
            if (args[i].equals("-size"))
                SZ = Integer.parseInt(args[++i]);
            if (args[i].equals("-save"))
                save = true;
        }
        if (seed.equals("1") && SZ < 100)
            SZ = 100;
        if (save) {
            fileName = seed;
            vis = true;
        }
        SZX = SZY = 2 * (SZ + D);
        PointsOnTheCircleVis f = new PointsOnTheCircleVis(seed);
    }

    // ---------------------------------------------------
    void addFatalError(String message) {
        System.out.println(message);
    }
}

class ErrorReader extends Thread {
    InputStream error;

    public ErrorReader(InputStream is) {
        error = is;
    }

    public void run() {
        try {
            byte[] ch = new byte[50000];
            int read;
            while ((read = error.read(ch)) > 0) {
                String s = new String(ch, 0, read);
                System.out.print(s);
                System.out.flush();
            }
        } catch (Exception e) {
        }
    }
}
