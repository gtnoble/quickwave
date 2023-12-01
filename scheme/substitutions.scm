(load-from-path "template.scm")

(define make-schema-node
  (lambda* (nodes precursors #:optional rules)
           (let ((nodes-dependencies
                   (map 
                     (lambda (make-schema-node)
                       (make-schema-node nodes))
                     precursors))) 
             (make-dependency-node 
               #:rule rules 
               #:dependent-nodes nodes-dependencies))))


(define make-schema
  (lambda* (#:key rules precursors)
           (lambda* (#:optional nodes)
                    (if precursors
                        (make-schema-node nodes precursors rules)
                        (make-dependency-node #:rule rules #:dependent-nodes nodes)))))

(define double-schema
  (make-schema
    #:rules '((number-type . "double") 
              (math-function-suffix . "")
              (math-macro-suffix . "")
              (function-tag . "_double")
              (struct-type-suffix . "Double"))))

(define float-schema
  (make-schema
    #:rules '((number-type . "float") 
              (math-function-suffix . "f")
              (math-macro-suffix . "F")
              (function-tag . "_float")
              (struct-type-suffix . "Float"))))

(define number-schema
  (make-schema
    #:rules '((number-base-type . "${number-type}")
              (real-struct-type-suffix . "${struct-type-suffix}")
              (real-function-tag . "${function-tag}"))
    #:precursors (list double-schema float-schema)))


(define complex-number-schema
  (make-schema
    #:rules '((number-type . "${number-type} complex") 
              (struct-type-suffix . "Complex${struct-type-suffix}")
              (function-tag . "_complex_${number-base-type}"))
    #:precursors (list number-schema)))

(define real-number-schema
  (make-schema
    #:rules '((number-type . "${number-type}") 
              (struct-type-suffix . "Real${struct-type-suffix}")
              (function-tag . "_real_${number-base-type}"))
    #:precursors (list number-schema)))


(define numeric-schema
  (make-schema
    #:precursors (list complex-number-schema  
                          real-number-schema)))

(define vector-schema
  (make-schema
    #:rules '((vector-type . "Vector${struct-type-suffix}"))
    #:precursors (list numeric-schema)))

(define filter-schema
  (make-schema
    #:rules '((filter-type . "DigitalFilter${struct-type-suffix}")
              (window-function-type . "WindowFunction${real-struct-type-suffix}"))
    #:precursors (list vector-schema)))


(define moving-average-rules
  '((moving-average-type . "MovingAverage${struct-type-suffix}")))

(define moving-average-schema
  (make-schema
    #:rules moving-average-rules
    #:precursors (list vector-schema)))

(define oscillator-rules
  '((oscillator-type . "Oscillator${struct-type-suffix}")))

(define oscillator-schema
  (make-schema
    #:rules oscillator-rules
    #:precursors (list number-schema)))


(define pid-rules
  '((pid-type . "Pid${struct-type-suffix}")))

(define pid-schema
  (make-schema
    #:rules pid-rules
    #:precursors (list number-schema)))


(define pll-schema
  (make-schema
    #:rules (append 
              oscillator-rules 
              pid-rules 
              '((pll-type . "PhaseLockedLoop${struct-type-suffix}")))
    #:precursors (list number-schema)))


(define sinusoid-fit-rules
  '((sinusoid-fit-type . "SinusoidFit${struct-type-suffix}")))

(define sinusoid-fit-schema
  (make-schema
    #:rules (append oscillator-rules moving-average-rules sinusoid-fit-rules)
    #:precursors (list number-schema)))

(define window-schema
  (make-schema
    #:rules '((window-function-type . "WindowFunction${struct-type-suffix}"))
    #:precursors (list number-schema)))

(define fft-schema 
  (make-schema
    #:rules '((fft-type . "FftComplex${struct-type-suffix}")
              (vector-type . "VectorComplex${struct-type-suffix}")
              (complex-function-tag . "_complex${function-tag}")
              (complex-number-type . "${number-type} complex"))
    #:precursors (list number-schema)))
