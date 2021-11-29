import java.util.*;

// It seems like a solution to move forward would be to use more maths :
// For example, to try A = 40 in simple P[i] - A[i, 1]X would be to try 27 - 40*6^1
// This can help up imagine graphs for each calculus that will only increase based of value of A.
// If we can figure out where this graphs intesects, then we don't have to do the calculus but can take this value.
// And assume it's the correct one. Then we don't need to try for every single value of A.

/**
 * Agent object is used to store the agent code and encoded message.
 * It doesn't have any thinking, only store attributes, basically a C struct.
 */
class Agent {
    private int m_code;
    private List<Integer> m_message;

    /**
     * Agent constructor. Simply take it's code and encoded message.
     * @param agent_code int value of the agent (7 for 007).
     * @param agent_message array of int, value of the encoded message owned by the agent, once first decrypted into int.
     */
    public Agent(int agent_code, List<Integer> agent_message)
    {
        m_code = agent_code;
        m_message = agent_message;
    }

    /**
     * Getter for agent code.
     * @return int agent code.
     */
    public int getCode()
    {
        return (m_code);
    }

    /**
     * Getter for agent message.
     * @return array of int agent message.
     */
    public List<Integer> getMessage()
    {
        return (m_message);
    }
}

/**
 * Solution class, required by CodinGame.
 */
class Solution {
    // Encoding / Decoding string for message as provided by CodinGame.
    static String all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

    /**
     * Check if all value inside array of int are equals.
     * @param array array of int to check.
     * @return true if all value are equals, false otherwise. True if empty.
     */
    private static boolean arrayAllEquals(List<Integer> array)
    {
        if (array.size() == 0) {
            return (true);
        }

        for (int i = 0; i < array.size(); i++) {
            if (array.get(i) != array.get(0)) {
                return (false);
            }
        }
    
        return (true);
    }

    /**
     * Transform int message to string based on CG String.
     * @param messageEncoded array of int of encoded message.
     * @return String of decoded message.
     */
    private static String decipherMessage(List<Integer> messageEncoded)
    {
        String messageDecoded = "";

        for (int n : messageEncoded) {
            messageDecoded += all.charAt(n);
        }

        return (messageDecoded);
    }

    /**
     * The math part of the problem, as explained by CodinGame.
     * P[i] = A[i,k-1]⋅X^(k-1) + A[i,k-2]⋅X^(k-2) + ... + A[i,1]⋅X + S[i] to encrypt.
     * So :
     * S[i] = P[i] - (A[i,k-1]⋅X^(k-1) + A[i,k-2]⋅X^(k-2) + ... + A[i,1]⋅X) to decrypt.
     * @param agent agent we are currently trying to decipher.
     * @param letter_index index of the letter we are currently trying to decipher.
     * @param As All A possibilities we are currently testing. Read "All of A".
     * @return The value of calculus given based on A values.
     */
    private static int solve_equation(Agent agent, int letter_index, List<Integer> As)
    {
        int integer_code = agent.getMessage().get(letter_index);
        int agent_code = agent.getCode();
        int sum_to_remove = 0;

        for (int i = 0; i < As.size(); i++) {
            // A[i,k-1]⋅X^(k-1) : A[i,k-1] = As.get(i), X = agent_code, ^(k-1) = pow(X, i + 1)
            sum_to_remove += As.get(i) * Math.pow(agent_code, i + 1);
        }

        int res = Math.floorMod(integer_code - sum_to_remove, 53);
        return (res);
    }

    /**
     * Function to figure out what is the correct A value for the given letter,
     * and as such figure out the correct letter of original (non encoded) message.
     * <p>
     * Generate all possibilities of differnt A's in current letter and try them one by one.
     * @param agents list of all agents that we were currently given.
     * @param letter_index index of letter we are currently trying to decipher.
     * @return int value of letter from original message.
     */
    private static int try_letter(List<Agent> agents, int letter_index)
    {
        List<Integer> solutions = new ArrayList<Integer>();

        // List of all possibilities of value A, read "All of A".
        List<Integer> As = new ArrayList<Integer>();

        int i_val;
        
        // Make sure that A's array have enough row to store all A possibilities.
        for (int i = 0; i < agents.size() - 1; i++) {
            As.add(0);
        }

        // for i from 0 to (amount of A possibilities) 53^(k-1)
        for (int i = 0; i < Math.pow(53, agents.size() - 1); i++) {

            // Figure out all value of A currently in testing.
            // If i = 140830,
            // As[0] = 140830 % 53 = 9,
            // As[1] = (140828 / 53) % 53 = 2657 % 53 = 7,
            // As[2] = (140828 / 53 / 53) = 2657 / 53 = 50.
            i_val = i;
            for (int j = 0; j < agents.size() - 1; j++) {
                if (i_val < 53) {
                    As.set(j, i_val);
                    break;
                }
                As.set(j, i_val % 53);
                i_val = i_val / 53;
                continue;
            }

            // Test current sets of A's.
            for (Agent agent : agents) {
                solutions.add(solve_equation(agent, letter_index, As));
            }

            // Return if current set of A's is working
            if (arrayAllEquals(solutions)) {
                System.err.println("Found solution for value of i : " + i + ", letter : " + all.charAt(solutions.get(0)));
                return (solutions.get(0));
            }

            // Make sure we empty so that there is no contamination from previous trys.
            solutions.clear();
        }

        return (-1);
    }

    /**
     * Loop on all letters of message to figure out the correct decoded message.
     * @param agents list of all agents that have messages.
     * @return list of integer that is the decoded final message from all agents message.
     */
    private static List<Integer> loop_letters(List<Agent> agents)
    {
        List<Integer> res = new ArrayList<Integer>();

        for (int i = 0; i < agents.get(0).getMessage().size(); i++) {
            res.add(try_letter(agents, i));
        }
        return (res);
    }

    /**
     * Transform String message into it's integer counterpart based on CG encoding String.
     * @param m string message owned by an agent.
     * @return list of integer that correspond to String m.
     */
    private static List<Integer> message_to_int(String m)
    {
        List<Integer> int_m = new ArrayList<Integer>();

        for (char c : m.toCharArray()) {
            int_m.add(all.indexOf(c));
        }

        return (int_m);
    }

    /**
     * Function to parse all agents from input.
     * @param n int number of agents.
     * @param in scanner object to read input from I/O.
     * @return list of all agents parsed.
     */
    private static List<Agent> get_all_agents(int n, Scanner in)
    {
        List<Agent> agents = new ArrayList<Agent>();

        int code;
        String message;
        List<Integer> int_message;
        
        for (int i = 0; i < n; i++) {
            code = in.nextInt();
            message = in.next();
            int_message = message_to_int(message);
            agents.add(new Agent(code, int_message));
        }
        
        return (agents);
    }

    /**
     * main function called by CG to test my solution
     * @param args String array args, java standards
     */
    public static void main(String args[]) {
        List<Agent> agents;
        List<Integer> res;
        Scanner in = new Scanner(System.in);
        int N = in.nextInt();

        agents = get_all_agents(N, in);

        res = loop_letters(agents);

        System.out.println(decipherMessage(res));
    }
}
