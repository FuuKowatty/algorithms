import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

class TopKFrequentElements {
    public static void main(String[] args) {
        int[] start = new int[]{1,2};
        System.out.println(Arrays.toString(topKFrequent(start, 2)));
    }

    public static int[] topKFrequent(int[] nums, int k) {
        Map<Integer, Integer> elements = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            Integer actualNumb = elements.getOrDefault(nums[i], 0);
            elements.put(nums[i], ++actualNumb);
        }
        ArrayList<Integer>[] buckets = initBuckets(nums.length);
        elements.entrySet().forEach(entry -> {
            buckets[entry.getValue()-1].add(entry.getKey());
        });
        int[] topElements = new int[k];
        int topElementsIdx = 0;
        for (int i = buckets.length-1; i >= 0; i--) {
            for (int j = 0; j < buckets[i].size(); j++) {
                topElements[topElementsIdx++] = buckets[i].get(j);
                if (topElementsIdx == k) {
                    return topElements;
                }
            }

        }
        return topElements;
    }

    @SuppressWarnings("unchecked")
    public static ArrayList<Integer>[] initBuckets(int length) {
        ArrayList<Integer>[] buckets = new ArrayList[length];
        for (int i = 0; i < length; i++) {
            buckets[i] = new ArrayList<>();
        }
        return buckets;
    } 

    public static void logBuckets(ArrayList<Integer>[] buckets) {
        for (int i = buckets.length-1; i >= 0; i--) {
            for (int j = 0; j < buckets[i].size(); j++) {
                System.out.println("Row: " + i +"; column: " + j + " = " + buckets[i].get(j));
            }
        }
    }

}