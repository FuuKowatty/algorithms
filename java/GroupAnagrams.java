import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

// https://neetcode.io/problems/anagram-groups/question?list=neetcode150
class GroupAnagrams {

    public static void main(String[] args) {
        String[] str = new String[]{"act","pots","tops","cat","stop","hat"};
        System.out.println(groupAnagrams(str));
    }

    // original anwser
    public static List<List<String>> groupAnagrams(String[] strs) {
        List<List<String>> grouped = new ArrayList<>(strs.length);
        Set<Integer> alreadyChecked = new HashSet<>();
        for (int i = 0; i < strs.length; i++) {
            if (alreadyChecked.contains(i)) {
                continue;
            }
            grouped.add(new ArrayList<>());
            int idx = grouped.size()-1;
            grouped.get(idx).add(strs[i]);
            for (int j = i+1; j < strs.length; j++) {
                if (isAnagram(strs[i], strs[j])) {
                    grouped.get(idx).add(strs[j]);
                    alreadyChecked.add(j);
                }
            }
        }
        return grouped;
    }
    
    public static boolean isAnagram(String s, String t) {
        if (s.length() != t.length()) {
            return false;
        }
        int[] letters = new int[256];
        for (int i = 0; i < s.length(); i++) {
            letters[(int) s.charAt(i)] += 1;
        }
        for (int i = 0; i < t.length(); i++) {
            if (letters[(int) t.charAt(i)] <= 0) {
                return false;
            }
            letters[(int)t.charAt(i)] -= 1;
        }
        return true;
    }

}