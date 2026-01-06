import java.util.ArrayList;
import java.util.List;

public class EncodeAndDecodeString {
   public static void main(String[] args) {
    
   }

   
    public static String encode(List<String> strs) {
        StringBuilder builder = new StringBuilder();
        int[] lengths = new int[strs.size()];
        for (int i = 0; i < strs.size(); i++) {
            builder.append(strs.get(i));
            lengths[i] = strs.get(i).length();
        }
        StringBuilder metadata = new StringBuilder();
        for (int i = 0; i < strs.size(); i++) {
            metadata.append(lengths[i]);
            metadata.append(",");
        }
        metadata.append('|');
        return metadata.append(builder.toString()).toString();
    }

    public static List<String> decode(String str) {
        List<Integer> lenghts = new ArrayList<>();
        List<String> words = new ArrayList<>();
        StringBuilder c = new StringBuilder();
        int cursor = 0;
        for (int i = 0; i < str.length(); i++) {
            cursor++;
            if (str.charAt(i) == '|') {
                break;
            }
            if (str.charAt(i) == ',') {
                lenghts.add(Integer.valueOf(c.toString()));
                c.setLength(0);
                continue;
            }
            c.append(str.charAt(i));
        }
        for (Integer length: lenghts) {
            for (int i = 0; i < length; i++) {
                c.append(str.charAt(cursor++));
            }
            words.add(c.toString());
            c.setLength(0);
        }
        return words;
    }

}
