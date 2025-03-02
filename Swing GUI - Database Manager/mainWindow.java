
/**
 * Write a description of class Window here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */

import javax.swing.*;
import java.awt.*;

public class mainWindow
{
    private String[] inputText = {"Company Name","Position Title","Location of Role"}; 
    private String[] inputDropdown = {"Applied","Under Review", "Interview Scheduled", "Waiting for Response", "Position Closed"};
    
    private void textlabelPair(String name, JPanel panel, int y, GridBagConstraints cons, String textFill){
        JLabel label = new JLabel(name);
        cons.gridx = 0;
        cons.gridy = y;
        panel.add(label, cons);
        
        JTextField textField = new JTextField(15);
        cons.gridx = 1;
        panel.add(textField, cons);
    }
    
    private void dropdownMenu(String name, String[] options, JPanel panel, int y, GridBagConstraints cons) {
        JLabel label = new JLabel(name);
        cons.gridx = 0;
        cons.gridy = y;
        panel.add(label, cons);

        JComboBox<String> dropdown = new JComboBox<>(options);
        cons.gridx = 1;
        panel.add(dropdown, cons);
     }

    public void initializeInputs(JFrame window){
        JPanel inputs = new JPanel(new GridBagLayout());
        window.add(inputs);
        GridBagConstraints inputsConstraints = new GridBagConstraints();
        inputsConstraints.fill = GridBagConstraints.HORIZONTAL;
        inputsConstraints.insets = new Insets(10,15,5,15); 
    
        textlabelPair("Company Name:", inputs, 0,inputsConstraints,"Carleton");
        textlabelPair("Position Title:", inputs, 1,inputsConstraints,"Student");
        textlabelPair("Location", inputs, 2,inputsConstraints,"Ottawa");
       
        String[] options = {"Applied", "Interview Pending", "Waiting for Response", "Failed"};
        dropdownMenu("Status:", options, inputs, 3, inputsConstraints);
       
        window.add(inputs, BorderLayout.CENTER);
    }    
    
    public void initializeActions(JFrame window){
        JPanel actions = new JPanel(new GridBagLayout());
        window.add(actions);
        GridBagConstraints actionsConstraints = new GridBagConstraints();
        actionsConstraints.fill = GridBagConstraints.HORIZONTAL;
        actionsConstraints.insets = new Insets(10,15,5,15); 
        
        JButton submit = new JButton("Submit");
        actionsConstraints.gridx = 0;
        actionsConstraints.gridy = 0;
        actions.add(submit, actionsConstraints);
        
        JButton clear = new JButton("Clear");
        actionsConstraints.gridx = 1;
        actionsConstraints.gridy = 0;
        actions.add(clear, actionsConstraints);
        
        window.add(actions, BorderLayout.SOUTH);
    }
    
     
     /**
     * Constructor for objects of class Window
     */
    public void initializeWindow()
    {
        JFrame mainWindow = new JFrame("Job Application Manager");
        mainWindow.setSize(300, 250);
        mainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainWindow.setLayout(new BorderLayout());

                
        initializeActions(mainWindow);
        initializeInputs(mainWindow);
        
        mainWindow.setVisible(true);          
     }
}
