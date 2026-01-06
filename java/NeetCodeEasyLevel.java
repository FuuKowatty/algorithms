import java.util.HashMap;
import java.util.Map;

class NeetCodeEasyLevel {
    public static void main(String[] args) {
    }

    public static boolean hasDuplicate(int[] nums) {
        Map<Integer, Boolean> map1 = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            Boolean e = map1.get(nums[i]);
            if (e == null) {
                map1.put(nums[i], true);
            } else {
                return true;
            }
        }
        return false;
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

    public static int[] twoSum(int[] nums, int target) {
        for (int i = 0; i < nums.length; i++) {
            for (int j = i+1; j < nums.length; j++) {
                if (nums[i] + nums[j] == target) {
                    return new int[]{i, j};
                }
            }
        }
        return new int[]{}; // assuming always find pair so whatever
    }
    
    // after hint to use map
    public static int[] twoSum2(int[] nums, int target) {
        Map<Integer, Integer> numbers = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            int toFound = target - nums[i];
            Integer idx = numbers.get(toFound);
            if (idx != null) {
                return new int[]{idx, i};
            } else {
                numbers.put(nums[i], i);
            }
        }
        return new int[]{}; // assuming always find pair so whatever
    }

}