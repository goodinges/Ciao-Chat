/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Authentication.JAAS;

import java.security.Principal;

/**
 *
 * @author Kamran
 */
public class UsernamePrincipal implements Principal, java.io.Serializable {

    private String name;

    public UsernamePrincipal(String name) {
        if (name == null) {
            throw new NullPointerException("illegal null input");
        }

        this.name = name;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return (name);
    }

    @Override
    public boolean equals(Object o) {
        if (o == null) {
            return false;
        }

        if (this == o) {
            return true;
        }

        if (!(o instanceof UsernamePrincipal)) {
            return false;
        }
        UsernamePrincipal that = (UsernamePrincipal) o;

        if (this.getName().equals(that.getName())) {
            return true;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return name.hashCode();
    }
}
