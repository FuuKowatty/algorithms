import java.util.Arrays;

// https://neetcode.io/problems/products-of-array-discluding-self/history
public class ProductsofArrayExceptSelf {
    public static void main(String[] args) {
        int[] nums = {1,2,4,6};
        productExceptSelfWithPrefix(nums);
    }
    // with division operation
    public static int[] productExceptSelf(int[] nums) {
        int[] res = new int[nums.length];
        int product = 1;
        boolean containsZero = false;
        for (int i = 0; i < nums.length; i++) {
            if (nums[i] == 0) {
                if (containsZero) {
                    return new int[nums.length];
                }
                containsZero = true;
            } else {
                product *= nums[i];
            }
        }
        for (int i = 0; i < nums.length; i++) {
            if (nums[i] == 0) {
                res[i] = product;
            } else {
                res[i] = containsZero ? 0 : product / nums[i];
            }
        }
        return res;
    }

    // without divsion operation
    public static int[] productExceptSelfWithPrefix(int[] nums) {
        int[] prefix = new int[nums.length];
        prefix[0] = 1;
        for (int i = 1; i < nums.length; i++) {
            prefix[i] = nums[i-1] * prefix[i-1]; 
        }
        int[] suffix = new int[nums.length];
        suffix[nums.length-1] = 1;
        for (int i = nums.length-2; i >= 0 ; i--) {
            suffix[i] = suffix[i+1] * nums[i+1];
        }
        int[] result = new int[nums.length]; 
        for (int i = 0; i < nums.length ; i--) {
            result[i] = suffix[i] * prefix[i];
        }
        return result;
    }

}

