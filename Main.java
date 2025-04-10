import java.io.RandomAccessFile;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;

class Show {

    private String show_Id;
    private String type;
    private String title;
    private String[] director;
    private String[] cast;
    private String[] country;
    private Date date;
    private Integer release_year;
    private String rating;
    private String duration;
    private String[] listen_in;

    // Construtor padrão com arrays preenchidos
    public Show() {
        this("", "", "", new String[] { "NaN" }, new String[] { "NaN" }, new String[] { "NaN" }, new Date(), 0, "", "", new String[] { "NaN" });
    }

    // Construtor de cópia
    public Show(Show objeto) {
        this.show_Id = objeto.show_Id;
        this.type = objeto.type;
        this.title = objeto.title;
        this.director = objeto.director != null ? objeto.director.clone() : null;
        this.cast = objeto.cast != null ? objeto.cast.clone() : null;
        this.country = objeto.country != null ? objeto.country.clone() : null;
        this.date = objeto.date != null ? (Date) objeto.date.clone() : null;
        this.release_year = objeto.release_year;
        this.rating = objeto.rating;
        this.duration = objeto.duration;
        this.listen_in = objeto.listen_in != null ? objeto.listen_in.clone() : null;
    }

    public Show(String show_Id, String type, String title, String[] director, String[] cast, String[] country,
            Date date, int release_year, String rating, String duration, String[] listen_in) {
        this.show_Id = show_Id;
        this.type = type;
        this.title = title;
        this.director = director;
        this.cast = cast;
        this.country = country;
        this.date = date;
        this.release_year = release_year;
        this.rating = rating;
        this.duration = duration;
        this.listen_in = listen_in;
    }

    /* ===================== MÉTODOS SETS ===================== */

    public void setShow_Id(String show_Id) { this.show_Id = show_Id; }
    public void setType(String type) { this.type = type; }
    public void setTitle(String title) { this.title = title; }
    public void setDirector(String[] director) { this.director = director; }
    public void setCast(String[] cast) { this.cast = cast; }
    public void setCountry(String[] country) { this.country = country; }
    public void setDate(Date date) { this.date = date; }
    public void setReleaseYear(int release_year) { this.release_year = release_year; }
    public void setRating(String rating) { this.rating = rating; }
    public void setDuration(String duration) { this.duration = duration; }
    public void setListenIn(String[] listen_in) { this.listen_in = listen_in; }

    /* ==================== MÉTODOS GETS ==================== */

    public String getShow_Id() { return this.show_Id; }
    public String getType() { return this.type; }
    public String getTitle() { return this.title; }
    public String[] getDirector() { return this.director; }
    public String[] getCast() { return this.cast; }
    public String[] getCountry() { return this.country; }
    public Date getDate() { return this.date; }
    public int getReleaseYear() { return this.release_year; }
    public String getRating() { return this.rating; }
    public String getDuration() { return this.duration; }
    public String[] getListenIn() { return this.listen_in; }

    /* ================== CLONE ===================== */

    public Show cloneClasse(Show object) {
        return new Show(object); // utiliza o construtor de cópia
    }

    public void printShowComplete() {
        System.out.println(
            "=> " + show_Id + " ## " + type + " ## " + title + " ## " + Arrays.toString(director) + " ## "
            + Arrays.toString(cast) + " ## " + Arrays.toString(country) + " ## " + (date == null ? "NaN" : date)
            + " ## " + (release_year == null ? "NaN" : release_year) + " ## " + rating + " ## " + duration
            + " ## " + Arrays.toString(listen_in));
    }

    public static Show[] readCSV() {
        ArrayList<Show> lista = new ArrayList<>();

        try {
            RandomAccessFile arqPtr = new RandomAccessFile("tmp/disneyplus.csv", "r");
            arqPtr.readLine(); // pula o cabeçalho

            String linhaAtual;
            while ((linhaAtual = arqPtr.readLine()) != null) {

                StringBuilder campo = new StringBuilder();
                boolean dentroAspas = false;
                String[] str = new String[12];
                int j = 0;

                for (int i = 0; i < linhaAtual.length(); i++) {
                    char c = linhaAtual.charAt(i);
                    if (c == '"') {
                        dentroAspas = !dentroAspas;
                    } else if (c == ',' && !dentroAspas) {
                        str[j++] = campo.toString().trim();
                        campo.setLength(0);
                    } else {
                        campo.append(c);
                    }
                }

                if (j < str.length) {
                    str[j] = campo.toString().trim();
                }

                if (str[0] == null || str[0].isEmpty())
                    continue;

                Show show = new Show();

                show.setShow_Id(str[0]);
                show.setType(str[1]);
                show.setTitle(str[2]);
                show.setDirector((str[3] == null || str[3].isEmpty()) ? new String[] { "NaN" } : str[3].split(", "));
                show.setCast((str[4] == null || str[4].isEmpty()) ? new String[] { "NaN" } : str[4].split(", "));
                show.setCountry((str[5] == null || str[5].isEmpty()) ? new String[] { "NaN" } : str[5].split(", "));

                try {
                    SimpleDateFormat dateFormat = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
                    show.setDate(str[6].isEmpty() ? null : dateFormat.parse(str[6]));
                } catch (Exception e) {
                    show.setDate(null);
                }

                show.setReleaseYear((str[7] == null || str[7].isEmpty()) ? 0 : Integer.parseInt(str[7]));
                show.setRating(str[8]);
                show.setDuration(str[9]);
                show.setListenIn((str[10] == null || str[10].isEmpty()) ? new String[] { "NaN" } : str[10].split(", "));

                // Ordena o cast
                String[] elenco = show.getCast();
                Show.sort(elenco);
                show.setCast(elenco);

                lista.add(show);
            }

            arqPtr.close();

        } catch (Exception e) {
            e.printStackTrace();
        }

        return lista.toArray(new Show[0]);
    }

    public static void swap(String[] array, int i, int j) {
        String temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    public static void sort(String[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            int menor = i;
            for (int j = i + 1; j < array.length; j++) {
                if (array[j].compareTo(array[menor]) < 0) {
                    menor = j;
                }
            }
            swap(array, i, menor);
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Show[] vetor = Show.readCSV();

        for (Show s : vetor) {
            s.printShowComplete();
        }
    }
}
