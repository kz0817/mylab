package myprocessor;

import java.lang.annotation.Target;
import java.util.Set;
import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.SupportedAnnotationTypes;
import javax.annotation.processing.SupportedSourceVersion;
import javax.lang.model.element.TypeElement;
import javax.lang.model.SourceVersion;
import javax.annotation.processing.RoundEnvironment;
import javax.tools.Diagnostic.Kind;

@SupportedSourceVersion(SourceVersion.RELEASE_17)
@SupportedAnnotationTypes({"ann.MyAnnotation"})
public class MyProcessor extends AbstractProcessor {
    @Override
    public void init(ProcessingEnvironment env) {
        super.init(env);
        env.getMessager().printMessage(Kind.NOTE, "INIT");
    }

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        processingEnv.getMessager().printMessage(Kind.NOTE, "process");
        return true;
    }
}

