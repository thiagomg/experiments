package main

import (
	"fmt"
	"io"
	"os"
)

func printLines1(writer io.Writer) error {

	var e error
	_, e = fmt.Fprintln(writer, "Line 1")
	if e != nil {
		return e
	}
	_, e = fmt.Fprintln(writer, "Line 2")
	if e != nil {
		return e
	}
	_, e = fmt.Fprintln(writer, "Line 3")
	if e != nil {
		return e
	}
	_, e = fmt.Fprintln(writer, "Line 4")
	if e != nil {
		return e
	}

	return nil
}

func printLines2(writer io.Writer) error {

	printer := NewPrinter(writer)
	printer.
		Ln("Line 1").
		Ln("Line 2").
		Ln("Line 3").
		Ln("Line 4")

	return printer.Err()
}

type Printer interface {
	Err() error
	Ln(a ...interface{}) Printer
}

type printer struct {
	writer io.Writer
	err    error
}

func NewPrinter(writer io.Writer) Printer {
	return &printer{writer: writer}
}

func (w *printer) Ln(a ...interface{}) Printer {
	// Let's only execute if no previous errors were seen
	if w.err == nil {
		_, w.err = fmt.Fprintln(w.writer, a...)
	}

	return w
}

func (w *printer) Err() error {
	return w.err
}

func main() {

	printLines1(os.Stdout)
	printLines2(os.Stdout)

}
