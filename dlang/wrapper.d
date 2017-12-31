import std.stdio;
import std.traits;

enum invokable;

mixin template wrap_class(alias source_class, string instance_name) {

    mixin(_wrap(typeof(new source_class).stringof));

    private string _wrap(string class_name) {
        string code = "" ~
            "class _proxy_" ~ class_name ~ "_ { " ~
            "  " ~ class_name ~ " _inst_ = new " ~ class_name ~ "; ";

        foreach(member_name; __traits(allMembers, source_class)) {
            static if(hasUDA!(__traits(getMember, source_class, member_name), invokable)) {
                string f = "__traits(getMember, " ~ class_name ~ ", \"" ~ member_name ~ "\")";
                code ~= "std.traits.ReturnType!(" ~ f ~ ") " ~ member_name ~ "(std.traits.ParameterTypeTuple!(" ~ f ~ ") args) {
                                import std.stdio;
                                writeln(\"[Invoking " ~ member_name ~"]\");
                                /* return */
                                return _inst_." ~ member_name ~ "(args);
                        }";
            }
        }
        code ~= "}";
        code ~= "auto " ~ instance_name ~ " = new _proxy_" ~ class_name ~ "_(); ";

        return code;
    }
}

class teen_writer {
    @invokable
    public void say(string text) {
        writeln("Whatever, " ~ text);
    }
}

auto wrapped(T)() {
    mixin wrap_class!(T, "_local_var_");
    return _local_var_;
}

void main()
{
    auto teenager = wrapped!teen_writer;
    teenager.say("Thiago");
}
