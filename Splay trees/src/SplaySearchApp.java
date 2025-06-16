import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.text.Collator;
import java.util.*;
import java.util.List;

public class SplaySearchApp extends JFrame {

    private SplayTree splayTree = new SplayTree();
    private JTextField searchField;
    private DefaultListModel<String> suggestionsModel;
    private JList<String> suggestionsList;
    private JLabel topQueriesLabel;
    private Map<String, List<String>> synonyms = new HashMap<>();

    public SplaySearchApp() {
        setTitle("Splay Search");
        setSize(600, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        loadQueriesFromFile("queries.txt");

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.setBackground(Color.WHITE);

        JPanel searchPanel = new JPanel();
        searchPanel.setLayout(new BoxLayout(searchPanel, BoxLayout.X_AXIS));
        searchPanel.setBackground(Color.WHITE);

        searchField = new JTextField();
        searchField.setMaximumSize(new Dimension(500, 40));
        searchField.setFont(new Font("SansSerif", Font.PLAIN, 24));

        JButton searchButton = new JButton("Поиск");
        searchButton.setFont(new Font("SansSerif", Font.PLAIN, 20));
        searchButton.addActionListener(e -> performSearch());

        searchPanel.add(searchField);
        searchPanel.add(Box.createRigidArea(new Dimension(10, 0)));
        searchPanel.add(searchButton);

        suggestionsModel = new DefaultListModel<>();
        suggestionsList = new JList<>(suggestionsModel);
        suggestionsList.setFont(new Font("SansSerif", Font.PLAIN, 20));
        suggestionsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        topQueriesLabel = new JLabel("<html>Последние запросы:<br>Запросов пока нет</html>");
        topQueriesLabel.setFont(new Font("SansSerif", Font.PLAIN, 16));

        searchField.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_DOWN && !suggestionsModel.isEmpty()) {
                    suggestionsList.requestFocus();
                    suggestionsList.setSelectedIndex(0);
                }
            }

            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_BACK_SPACE) {
                    updateSuggestions(searchField.getText(), false);
                } else if (e.getKeyCode() != KeyEvent.VK_DOWN && e.getKeyCode() != KeyEvent.VK_UP) {
                    updateSuggestions(searchField.getText(), true);
                }
            }
        });



        suggestionsList.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    String selected = suggestionsList.getSelectedValue();
                    if (selected != null) {
                        searchField.setText(selected);
                        suggestionsList.clearSelection();
                        searchField.requestFocus();
                        updateSuggestions(searchField.getText(), false);
                    }
                }
            }
        });

        suggestionsList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                String selected = suggestionsList.getSelectedValue();
                if (selected != null) {
                    searchField.setText(selected);
                    suggestionsList.clearSelection();
                    searchField.requestFocus();
                    updateSuggestions(searchField.getText(), false);
                }
            }
        });

        mainPanel.add(Box.createVerticalGlue());
        mainPanel.add(searchPanel);
        mainPanel.add(Box.createRigidArea(new Dimension(0, 10)));
        mainPanel.add(new JScrollPane(suggestionsList));
        mainPanel.add(Box.createRigidArea(new Dimension(0, 10)));
        mainPanel.add(topQueriesLabel);
        mainPanel.add(Box.createVerticalGlue());

        add(mainPanel);
        setVisible(true);
    }

    private void performSearch() {
        String text = searchField.getText().trim();
        if (!text.isEmpty()) {
            if (splayTree.exists(text)) {
                splayTree.incrementCount(text);
            } else {
                splayTree.insert(text);
                splayTree.incrementCount(text);
            }
            updateSuggestions(text, false);
            updateTopQueries();
            searchField.setText("");
        }
    }

    private void updateTopQueries() {
        List<SplayTree.Entry> entries = new ArrayList<>();
        splayTree.collectAll(entries);
        entries.removeIf(entry -> entry.count == 0);
        entries.sort((a, b) -> Integer.compare(b.count, a.count));

        if (entries.isEmpty()) {
            topQueriesLabel.setText("<html>Последние запросы:<br>Запросов пока нет</html>");
            return;
        }

        StringBuilder sb = new StringBuilder("<html>Последние запросы:<br>");

        for (int i = 0; i < Math.min(3, entries.size()); i++) {
            SplayTree.Entry e = entries.get(i);
            sb.append((i + 1)).append(". ").append(e.word).append(" — ").append(e.count).append(" раз;<br>");
        }
        sb.append("</html>");
        topQueriesLabel.setText(sb.toString());
    }

    private void loadQueriesFromFile(String filename) {
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = br.readLine()) != null) {
                if (!line.trim().isEmpty()) {
                    splayTree.insert(line.trim());
                }
            }
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Не удалось загрузить файл запросов.");
            e.printStackTrace();
        }
    }



    private void updateSuggestions(String prefix, boolean autoComplete) {
        suggestionsModel.clear();
        if (prefix.isEmpty()) {
            return;
        }

        Set<String> matchesSet = new LinkedHashSet<>();
        ArrayList<String> matches = new ArrayList<>();
        splayTree.collectMatches(prefix, matches);
        matchesSet.addAll(matches);

        if (synonyms.containsKey(prefix)) {
            for (String synonym : synonyms.get(prefix)) {
                ArrayList<String> synonymMatches = new ArrayList<>();
                splayTree.collectMatches(synonym, synonymMatches);
                matchesSet.addAll(synonymMatches);
            }
        }

        List<String> sortedMatches = new ArrayList<>(matchesSet);
        sortedMatches.sort((a, b) -> Integer.compare(splayTree.getCount(b), splayTree.getCount(a)));

        for (String match : sortedMatches) {
            suggestionsModel.addElement(match);
        }

        if (autoComplete && !sortedMatches.isEmpty()) {
            String firstMatch = sortedMatches.get(0);
            if (firstMatch.startsWith(prefix) && !firstMatch.equals(prefix)) {
                searchField.setText(firstMatch);
                searchField.setCaretPosition(prefix.length());
                searchField.moveCaretPosition(firstMatch.length());
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(SplaySearchApp::new);
    }
}

class SplayTree {

    static class Node {
        String word;
        Node left, right;
        int count;

        Node(String word) {
            this.word = word;
            left = right = null;
            count = 0;
        }
    }

    static class Entry {
        String word;
        int count;

        Entry(String word, int count) {
            this.word = word;
            this.count = count;
        }
    }

    private Node root;
    private Collator collator = Collator.getInstance(new Locale("ru", "RU"));

    private Node rightRotate(Node x) {
        Node y = x.left;
        x.left = y.right;
        y.right = x;
        return y;
    }

    private Node leftRotate(Node x) {
        Node y = x.right;
        x.right = y.left;
        y.left = x;
        return y;
    }

    private Node splay(Node root, String key) {
        if (root == null || root.word.equals(key))
            return root;

        if (collator.compare(key, root.word) < 0) {
            if (root.left == null) return root;

            if (collator.compare(key, root.left.word) < 0) {
                root.left.left = splay(root.left.left, key);
                root = rightRotate(root);
            } else if (collator.compare(key, root.left.word) > 0) {
                root.left.right = splay(root.left.right, key);
                if (root.left.right != null)
                    root.left = leftRotate(root.left);
            }

            return (root.left == null) ? root : rightRotate(root);
        } else {
            if (root.right == null) return root;

            if (collator.compare(key, root.right.word) > 0) {
                root.right.right = splay(root.right.right, key);
                root = leftRotate(root);
            } else if (collator.compare(key, root.right.word) < 0) {
                root.right.left = splay(root.right.left, key);
                if (root.right.left != null)
                    root.right = rightRotate(root.right);
            }

            return (root.right == null) ? root : leftRotate(root);
        }
    }

    public void insert(String key) {
        if (root == null) {
            root = new Node(key);
            return;
        }

        root = splay(root, key);

        if (root.word.equals(key))
            return;

        Node newNode = new Node(key);

        if (collator.compare(key, root.word) < 0) {
            newNode.right = root;
            newNode.left = root.left;
            root.left = null;
        } else {
            newNode.left = root;
            newNode.right = root.right;
            root.right = null;
        }

        root = newNode;
    }

    public boolean exists(String key) {
        Node temp = find(root, key);
        return temp != null;
    }

    public void collectMatches(String prefix, ArrayList<String> matches) {
        collectMatchesHelper(root, prefix, matches);
    }

    private void collectMatchesHelper(Node node, String prefix, ArrayList<String> matches) {
        if (node == null) return;
        if (node.word.startsWith(prefix)) matches.add(node.word);
        collectMatchesHelper(node.left, prefix, matches);
        collectMatchesHelper(node.right, prefix, matches);
    }

    public void collectAll(List<Entry> entries) {
        collectAllHelper(root, entries);
    }

    private void collectAllHelper(Node node, List<Entry> entries) {
        if (node == null) return;
        entries.add(new Entry(node.word, node.count));
        collectAllHelper(node.left, entries);
        collectAllHelper(node.right, entries);
    }

    public void incrementCount(String key) {
        root = splay(root, key);
        if (root != null && root.word.equals(key)) {
            root.count++;
        }
    }

    public int getCount(String key) {
        Node temp = find(root, key);
        return temp == null ? 0 : temp.count;
    }

    private Node find(Node node, String key) {
        if (node == null) return null;
        if (node.word.equals(key)) return node;
        if (collator.compare(key, node.word) < 0) return find(node.left, key);
        else return find(node.right, key);
    }
}
