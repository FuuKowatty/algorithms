

// You have an array of integers. Find an index where the sum of elements to the 
// left is equal to the sum of elements to the right. If multiple indices exist, 
// return the first one. If none, return -1.
public class FindLeftRightSum {
    public static void main(String[] args) {
        int[] input = new int[]{1, 7, 3, 6, 5, 6};
        System.out.println(findPivotIndex(input));
    }

    static int findPivotIndex(int[] numbs) {
        int[] prefix = new int[numbs.length];
        prefix[0] = numbs[0];
        for (int i = 1; i < numbs.length; i++) {
            prefix[i] = numbs[i] + prefix[i-1];
        }
        for (int i = 1; i < prefix.length; i++) {
            if (prefix[i-1] == prefix[prefix.length-1] - prefix[i]) {
                return i;
            };  
        }
        return -1;
    }
}
