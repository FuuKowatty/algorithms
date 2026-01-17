
// https://neetcode.io/problems/max-water-container/question?list=neetcode150
class MaxArea {
    public static void main(String[] args) {

    }

    public static int maxArea(int[] heights) {
        int left = 0;
        int right = heights.length-1;
        int max = 0;
        while (left < right) {
            boolean isRightSmaller = heights[left] > heights[right];
            int height = isRightSmaller ? heights[right] : heights[left];
            int score = height * (right - left);
            max = max < score ? score : max;
            if (isRightSmaller) {
                right--;
            } else {
                left++;
            }
        }
        return max;
    }

}