import java.util.HashMap;
import java.util.TreeMap;
import java.io.FileWriter;
import java.io.IOException;

public class Benchmark {

    // Clase interna que representa un estudiante
    static class Student {
        int studentId;
        String fullName;
        String career;
        int semester;
        float gpa;
        int skillScore;

        Student(int studentId, String fullName, String career,
                int semester, float gpa, int skillScore) {
            this.studentId = studentId;
            this.fullName  = fullName;
            this.career    = career;
            this.semester  = semester;
            this.gpa       = gpa;
            this.skillScore = skillScore;
        }
    }

    public static void main(String[] args) {

        final int N       = 10000;
        final int ID_BASE = 20001;

        // ── Generar 10000 estudiantes sinteticos ──────────────────────────────
        Student[] estudiantes = new Student[N];
        for (int i = 0; i < N; i++) {
            int id = ID_BASE + i;
            estudiantes[i] = new Student(
                id,
                "Estudiante " + id,
                "Ingenieria en Sistemas",
                id % 10 + 1,
                75.0f + (id % 25),
                id % 500
            );
        }

        double[] tiempos = new double[4];

        // ── Benchmark HashMap ─────────────────────────────────────────────────

        // a) Insercion en HashMap
        HashMap<Integer, Student> hashMap = new HashMap<>();
        long t0 = System.nanoTime();
        for (Student s : estudiantes) {
            hashMap.put(s.studentId, s);
        }
        tiempos[0] = (System.nanoTime() - t0) / 1_000_000.0;
        System.out.println("HashMap insert    : " + String.format("%.2f", tiempos[0]) + " ms");

        // b) Busqueda en HashMap
        t0 = System.nanoTime();
        for (int i = 0; i < N; i++) {
            hashMap.get(ID_BASE + i);
        }
        tiempos[1] = (System.nanoTime() - t0) / 1_000_000.0;
        System.out.println("HashMap search    : " + String.format("%.2f", tiempos[1]) + " ms");

        // ── Benchmark TreeMap ─────────────────────────────────────────────────

        // c) Insercion en TreeMap (clave = skillScore)
        TreeMap<Integer, Student> treeMap = new TreeMap<>();
        t0 = System.nanoTime();
        for (Student s : estudiantes) {
            treeMap.put(s.studentId, s);
        }
        tiempos[2] = (System.nanoTime() - t0) / 1_000_000.0;
        System.out.println("TreeMap insert    : " + String.format("%.2f", tiempos[2]) + " ms");

        // d) Recorrido inorden de TreeMap (entrySet ya esta ordenado)
        t0 = System.nanoTime();
        for (java.util.Map.Entry<Integer, Student> entry : treeMap.entrySet()) {
            // Acceder clave y valor para que el JIT no optimice el bucle
            int k = entry.getKey();
            Student v = entry.getValue();
        }
        tiempos[3] = (System.nanoTime() - t0) / 1_000_000.0;
        System.out.println("TreeMap traversal : " + String.format("%.2f", tiempos[3]) + " ms");

        // ── Escribir resultados a CSV ─────────────────────────────────────────
        String rutaCSV = "../dataset/java_results.csv";
        try (FileWriter fw = new FileWriter(rutaCSV)) {
            fw.write("language,operation,structure,records,time_ms\n");
            fw.write("Java,insert,HashMap,"   + N + "," + String.format(java.util.Locale.US, "%.2f", tiempos[0]) + "\n");
            fw.write("Java,search,HashMap,"   + N + "," + String.format(java.util.Locale.US, "%.2f", tiempos[1]) + "\n");
            fw.write("Java,insert,TreeMap,"   + N + "," + String.format(java.util.Locale.US, "%.2f", tiempos[2]) + "\n");
            fw.write("Java,traversal,TreeMap," + N + "," + String.format(java.util.Locale.US, "%.2f", tiempos[3]) + "\n");
        } catch (IOException e) {
            System.out.println("Error al escribir " + rutaCSV + ": " + e.getMessage());
            return;
        }

        System.out.println("Archivo java_results.csv generado correctamente.");
    }
}